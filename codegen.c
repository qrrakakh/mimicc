#include "mimicc.h"

//////////
// static value
char x86_64_argreg_8bits[][6]  = {"dil", "sil", "dl",  "cl",  "r8b", "r9b"};
char x86_64_argreg_16bits[][6] = {"di",  "si",  "dx",  "cx",  "r8w", "r9w"};
char x86_64_argreg_32bits[][6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
char x86_64_argreg_64bits[][6] = {"rdi", "rsi", "rdx", "rcx", "r8",  "r9" };
const int POINTER_SIZE_BYTES = 8;
const int STRUCT_ALL_ALIGN_BYTES = 4;

//////////
// type helper function
int GetTypeSize(Type *ty) {
  if(ty->kind == TYPE_CHAR || ty->kind == TYPE_BOOL) {
    return 1;
  } else if(ty->kind == TYPE_INT || ty->kind == TYPE_ENUM) {
    return 4;
  } else if(ty->kind == TYPE_PTR) {
    return 8;
  } else if(ty->kind == TYPE_ARRAY) {
    return ty->array_size * GetTypeSize(ty->ptr_to);
  } else if(ty->kind == TYPE_STRUCT) {
    return FindStructById(ty->id)->size;
  } else {
    Error("Unexpected type is input.");
    return -1;
  }
}

int GetSizeVar(Type *ty) {
  if(ty->kind == TYPE_CHAR || ty->kind == TYPE_BOOL) {
    return 1;
  } else if(ty->kind == TYPE_INT || ty->kind == TYPE_ENUM) {
    return 4;
  } else if(ty->kind == TYPE_PTR || ty->kind == TYPE_ARRAY) {
    return 8;
  } else {
    Error("Unexpected type is input.");
    return -1;
  }
}

int GetSizePtrTarget(Type *ty) {
  return GetSizeVar(ty->ptr_to);
}

int GetStructMemberOffset(int struct_id, int member_id) {
  Struct *s;
  Symbol *v;
  
  s = FindStructById(struct_id);
  for(v=s->members;v->next;v=v->next) {
    if(v->id == member_id) {
      return v->offset_bytes;
    }
  }
  Error("Member not found.");
  return -1;
}

//////////
// evaluator for constant

int eval(Node *node) {
  switch(node->kind) {
    case ND_ASSIGN:
      ErrorAt(node->tok->str, "Assignment is not a constant.");
    case ND_OR:
      return eval(node->children[0]) | eval(node->children[1]);
    case ND_XOR:
      return eval(node->children[0]) ^ eval(node->children[1]);
    case ND_AND:
      return eval(node->children[0]) & eval(node->children[1]);
    case ND_EQUIV:
      return eval(node->children[0]) == eval(node->children[1]);
    case ND_INEQUIV:
      return eval(node->children[0]) != eval(node->children[1]);
    case ND_LT:
      return eval(node->children[0]) < eval(node->children[1]);
    case ND_LE:
      return eval(node->children[0]) <= eval(node->children[1]);
    case ND_LSHIFT:
      return eval(node->children[0]) << eval(node->children[1]);
    case ND_RSHIFT:
      return eval(node->children[0]) >> eval(node->children[1]);
    case ND_MUL:
      return eval(node->children[0]) * eval(node->children[1]);
    case ND_DIV:
      return eval(node->children[0]) / eval(node->children[1]);
    case ND_MOD:
      return eval(node->children[0]) % eval(node->children[1]);
    case ND_ADD:
      return eval(node->children[0]) + eval(node->children[1]);
    case ND_SUB:
      return eval(node->children[0]) - eval(node->children[1]);
    case ND_NUM:
    case ND_CHAR:
      return node->val;
    default:
      break;
  }
  ErrorAt(node->tok->str, "Not a constant.");
  return 0;
}

//////////
// Code generator
void StoreVar(Type *ty, _Bool eval) {
  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch(GetSizeVar(ty)) {
    case 1:
      printf("  mov [rax], dil\n");
      break;
    case 2:
      printf("  mov [rax], di\n");
      break;
    case 4:
      printf("  mov [rax], edi\n");
      break;
    case 8:
      printf("  mov [rax], rdi\n");
      break;
  }

  if (eval) {
    printf("  mov rax, rdi\n");
  }
}

void LoadVar(Type *ty) {
  switch(GetSizeVar(ty)) {
    case 1:
      printf("  movsx rax, byte ptr [rax]\n");
      break;
    case 2:
      printf("  movsx rax, word ptr [rax]\n");
      break;
    case 4:
      printf("  movsxd rax, dword ptr [rax]\n");
      break;
    case 8:
      printf("  mov rax, [rax]\n");
      break;
  }
}

void GenLval(Node *node) {
  Symbol *symbol;
  if (node->kind == ND_LVAR || node->kind == ND_INIT) {
    // save the address of lval
    symbol = FindLvarById(node->id);
    printf("  lea rax, [rbp-%d]\n", symbol->offset_bytes);
  } else if(node->kind == ND_GVAR) {
    symbol = FindGvarById(node->id);
    printf("  lea rax, %.*s[rip]\n", symbol->len, symbol->name);
  } else if(node->kind == ND_DEREF) {
    Generate(node->children[0]);
  } else if(node->kind == ND_STRINGS) {
    printf("  lea rax, .LC%06d[rip]\n", node->id);
  } else if(node->kind == ND_ARROW) {
    Generate(node->children[0]);
    printf("  lea rax, [rax+%d]\n",
      GetStructMemberOffset(node->children[0]->ty->ptr_to->id, node->id));
  } else if(node->kind == ND_DOT) {
    GenLval(node->children[0]);
    printf("  lea rax, [rax+%d]\n",
      GetStructMemberOffset(node->children[0]->ty->id, node->id));
  } else {
    Error("lval is not a variable, %d", node->kind);
  }
}

void InitProgram() {
  // calculate size and offsets of each structs
  Struct *s = structs;
  Symbol *v;
  int var_size, diff;

  while(s->next) {
    for(v=s->members;v->next;v=v->next);

    s->size = 0;
    for(v=v->prev;v;v=v->prev) {
      v->offset_bytes = s->size;
      var_size = GetTypeSize(v->ty);
      s->size += var_size;
      diff = v->offset_bytes % var_size;
      if (diff>0) {
        v->offset_bytes += var_size - diff;
      }
      s->size = v->offset_bytes + var_size;
    }

    if (s->size % STRUCT_ALL_ALIGN_BYTES > 0) {
      s->size = (s->size / STRUCT_ALL_ALIGN_BYTES + 1) * STRUCT_ALL_ALIGN_BYTES;
    }

    s = s->next;
  }
  
  printf(".intel_syntax noprefix\n");
}

void GenerateFooter() {
  Symbol *g;
  Const_Strings *c;
  // global variable
  printf("  .data\n");
  for(g=globals;g->next!=NULL;g=g->next) {
    if(g->scope_id==-1) { // extern
      continue;
    }
    if(g->kind!=SY_VAR) { // not a variable
      continue;
    }
    printf("  .global %.*s\n", g->len, g->name);
    printf("%.*s:\n", g->len, g->name);
    if (g->ty->kind == TYPE_STRUCT) {
      printf("  .zero %d\n", GetTypeSize(g->ty));
    } else if(g->ty->kind == TYPE_ARRAY && g->ty->ptr_to->kind == TYPE_CHAR && g->initializer) {
      Const_Strings *cstr = FindCstrById(g->initializer->children[0]->id);
      for(int i=0;i<cstr->size;++i) {
        printf("  .byte %#x\n", (cstr->str)[i]);
      }
    } else if (g->ty->kind == TYPE_ARRAY) {
      for(int i=0;i<g->ty->array_size;++i) {
        printf("  .zero %d\n", GetTypeSize(g->ty->ptr_to));
      }
    } else if(g->initializer) {
      printf("  .%dbyte %d\n", GetTypeSize(g->ty), eval(g->initializer->children[0]));
    } else {
      printf("  .zero %d\n", GetTypeSize(g->ty));
    }
  }
  for(c=cstrs;c->next!=NULL;c=c->next) {
    if(c->is_global_init) continue;
    printf(".LC%06d:\n", c->id);
    printf("  .string \"%.*s\"\n", c->size, c->str);
  }
}


void Generate(Node *node) {
  int label;
  int lvar_area_size;
  int num_lvar;
  int lvar_idx;
  int diff;
  int prev_ctrl_depth;
  Node **stmt_list;
  Node *node_cur;
  Type *lhs_ty, *rhs_ty;
  Symbol *var;
  
  switch(node->kind) {
    case ND_DUMMY:
      return;
    case ND_FUNC:
    if (!(node->children[0]))
      return;
    printf("  .global %.*s\n", node->val, node->name);
    printf("%.*s:\n", node->val, node->name);
    current_func = FindFuncByName(node->name, node->val);

    locals = node->lvars;
    // allocate stack for local variables
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");

    // calculate required reserved stack area sizeand offsets for each lvars
    lvar_area_size = 0;
    num_lvar = 0;
    var = locals;
    while (var->next) {
      if(var->kind !=SY_VAR) {
        var = var->next;
        continue;
      }
      ++num_lvar;
      lvar_area_size += GetTypeSize(var->ty);
      var->offset_bytes = lvar_area_size;
      var = var->next;
    }

    //printf("  sub rsp, %d\n", get_num_lvars() * POINTER_SIZE_BYTES);
    printf("  sub rsp, %d\n", lvar_area_size);

    // copy passed argument values to the local variables
    var = locals;
    while(num_lvar > node->num_args) {
      var=var->next; --num_lvar;
    }

    for(int i=0;i<node->num_args;++i) {
      lvar_idx = node->num_args-1-i;
      switch(GetSizeVar(var->ty)) {
        case 1:
        printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_8bits[lvar_idx]);
        break;
        case 2:
        printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_16bits[lvar_idx]);
        break;
        case 4:
        printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_32bits[lvar_idx]);
        break;
        case 8:
        printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_64bits[lvar_idx]);
        break;
      }
      var = var->next;
    }


    Generate(node->children[0]);

    printf(".L.ret%.*s:\n", node->val, node->name);
    current_func = NULL;
    
    // Final evaluated value is already stored on rax, which will be returned.
    // To back to the original address, we fix the rsp
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");

    return;

    case ND_RETURN:
    if(node->children[0])
      Generate(node->children[0]);
    printf("  jmp .L.ret%.*s\n", current_func->symbol->len, current_func->symbol->name);
    return;

    case ND_BREAK:
    printf("  jmp .L.end%06d\n", ctrl_depth);
    return;

    case ND_CONTINUE:
    printf("  jmp .L.next%06d\n", ctrl_depth);
    return;

    case ND_BLOCK:
    stmt_list = node->children;
    while (*stmt_list != NULL) {
      Generate(*(stmt_list++));
    }
    printf("  mov rax, 0\n");
    return;

    case ND_ADDR:
    GenLval(node->children[0]);
    return;

    case ND_DEREF:
    Generate(node->children[0]);
    LoadVar(node->ty);
    return;

    case ND_ARROW:
    Generate(node->children[0]);
    printf("  lea rax, [rax+%d]\n",
      GetStructMemberOffset(node->children[0]->ty->ptr_to->id, node->id));
    LoadVar(node->ty);
    return;

    case ND_DOT:
    GenLval(node->children[0]);
    printf("  lea rax, [rax+%d]\n",
      GetStructMemberOffset(node->children[0]->ty->id, node->id));
    LoadVar(node->ty);
    return;

    case ND_NUM:
    case ND_CHAR:
    printf("  mov rax, %d\n", node->val);
    return;

    case ND_STRINGS:
    printf("  lea rax, .LC%06d[rip]\n", node->id);
    return;

    case ND_SIZEOF:
    // TODO: when we implement struct, judge if the var is struct or not
    printf("  mov rax,%d\n", GetTypeSize(node->children[0]->ty));
    return;

    case ND_NOT:
    Generate(node->children[0]);
    printf("  not eax\n");
    return;

    case ND_PREINC:
    diff = 1;
    if(node->children[0]->ty->kind == TYPE_PTR) {
      diff = GetSizePtrTarget(node->children[0]->ty);
    }
    GenLval(node->children[0]);
    printf("  push rax\n");
    LoadVar(node->children[0]->ty);
    printf("  add rax, %d\n", diff);
    printf("  push rax\n");
    StoreVar(node->children[0]->ty, 1);
    return;

    case ND_PREDEC:
    diff = 1;
    if(node->children[0]->ty->kind == TYPE_PTR) {
      diff = GetSizePtrTarget(node->children[0]->ty);
    }
    GenLval(node->children[0]);
    printf("  push rax\n");
    LoadVar(node->children[0]->ty);
    printf("  sub rax, %d\n", diff);
    printf("  push rax\n");
    StoreVar(node->children[0]->ty, 1);
    return;

    case ND_POSTINC:
    diff = 1;
    if(node->children[0]->ty->kind == TYPE_PTR) {
      diff = GetSizePtrTarget(node->children[0]->ty);
    }
    GenLval(node->children[0]);
    printf("  push rax\n");
    LoadVar(node->children[0]->ty);
    printf("  mov rsi, rax\n");
    printf("  add rax, %d\n", diff);
    printf("  push rax\n");
    StoreVar(node->children[0]->ty, 0);
    printf("  mov rax, rsi\n");
    return;

    case ND_POSTDEC:
    diff = 1;
    if(node->children[0]->ty->kind == TYPE_PTR) {
      diff = GetSizePtrTarget(node->children[0]->ty);
    }
    GenLval(node->children[0]);
    printf("  push rax\n");
    LoadVar(node->children[0]->ty);
    printf("  mov rsi, rax\n");
    printf("  sub rax, %d\n", diff);
    printf("  push rax\n");
    StoreVar(node->children[0]->ty, 0);
    printf("  mov rax, rsi\n");
    return;

    case ND_LIST:
    for(int i=0;i<node->num_args;++i) {
      Generate(node->children[i]);
    }
    return;

    case ND_GVAR:
    case ND_LVAR:
    GenLval(node);
    LoadVar(node->ty);
    return;

    case ND_INIT:
    GenLval(node);
    printf("  push rax\n");
    Generate(node->children[0]);
    printf("  push rax\n");
    StoreVar(node->ty, 1);
    return;

    case ND_ASSIGN:
    GenLval(node->children[0]);
    printf("  push rax\n");
    Generate(node->children[1]);
    printf("  push rax\n");
    StoreVar(node->ty, 1);
    return;

    case ND_WHILE:
    label = label_index++;
    prev_ctrl_depth = ctrl_depth;
    ctrl_depth = label;
    printf(".L.next%06d:\n", label);
    Generate(node->children[0]);
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    Generate(node->children[1]);
    printf("  jmp .L.next%06d\n", label);
    printf(".L.end%06d:\n", label);
    ctrl_depth = prev_ctrl_depth;
    return;

    case ND_FOR:
    label = label_index++;
    prev_ctrl_depth = ctrl_depth;
    ctrl_depth = label;
    Generate(node->children[0]);
    printf(".L.begin%06d:\n", label);
    Generate(node->children[1]);
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    Generate(node->children[3]);
    printf(".L.next%06d:\n", label);
    Generate(node->children[2]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    ctrl_depth = prev_ctrl_depth;
    return;

    case ND_SWITCH:
    label = label_index++;
    prev_ctrl_depth = ctrl_depth;
    ctrl_depth = label;
    Generate(node->children[0]);
    printf("  mov r10, rax\n");
    node_cur = node->children[1];
    while(node_cur) {
      printf("  cmp r10, %d\n", node_cur->children[0]->val);
      printf("  je .L.sw%06d.case%06d\n", ctrl_depth, node_cur->val);
      node_cur = node_cur->children[1];
    }
    if(node->num_args==0) { // default
      printf("  jmp .L.sw%06d.case%06d\n", ctrl_depth, 0);
    } else {
      printf("  jmp .L.end%06d\n", label);
    }
    Generate(node->children[2]);
    printf(".L.end%06d:\n", label);
    ctrl_depth = prev_ctrl_depth;
    return;

    case ND_SWLABEL:
    printf(".L.sw%06d.case%06d:", ctrl_depth, node->val);
    return;

    case ND_IF:
    label = label_index++;
    Generate(node->children[0]);
    printf("  cmp rax, 0\n"); // 0 if cond is not satisfied
    printf("  je .L.else%06d\n", label);
    Generate(node->children[1]);
    printf("  jmp .L.end%06d\n",  label);
    printf(".L.else%06d:\n",  label);
    if(node->children[2]!=NULL) {
      Generate(node->children[2]);
    }
    printf(".L.end%06d:\n",  label);
    return;

    case ND_CALL:
    for(int i=0;i<node->num_args;++i) {
      Generate(node->children[i]);
      switch(GetSizeVar(node->children[i]->ty)) {
        case 1:
        printf("  mov %s, al\n", x86_64_argreg_8bits[i]);
        break;  
        case 2:
        printf("  mov %s, ax\n", x86_64_argreg_16bits[i]);
        break;  
        case 4:
        printf("  mov %s, eax\n", x86_64_argreg_32bits[i]);
        break;  
        case 8:
        printf("  mov %s, rax\n", x86_64_argreg_64bits[i]);
        break;  
      }
    }

    // modify rsp as a multiply of 16
    label = label_index++;
    printf("  mov rax, rsp\n");
    printf("  and rax, 15\n");
    printf("  jnz .L.call%06d\n", label);
    printf("  mov rax, 0\n");
    printf("  mov r10, qword ptr %.*s@GOTPCREL[rip]\n", node->val, node->name);
    printf("  call r10\n");
    printf("  jmp .L.end%06d\n", label);
    printf(".L.call%06d:\n", label);
    printf("  sub rsp, 8\n");
    printf("  mov rax, 0\n");
    printf("  mov r10, qword ptr %.*s@GOTPCREL[rip]\n", node->val, node->name);
    printf("  call r10\n");
    printf("  add rsp, 8\n");
    printf(".L.end%06d:\n", label);
    return;
  }

  Generate(node->children[1]);
  rhs_ty = node->children[0]->ty;
  printf("  push rax\n");
  Generate(node->children[0]);
  lhs_ty = node->children[0]->ty;
  printf("  pop rdi\n");
  
  switch(node->kind) {
    case ND_ADD:
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",GetSizePtrTarget(lhs_ty));
      } else if(rhs_ty->kind == TYPE_PTR) {
        printf("  imul rax, %d\n",GetSizePtrTarget(rhs_ty));
      }
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:    
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",GetSizePtrTarget(lhs_ty));
      }
      printf("  sub rax, rdi\n");
      break;
    case ND_EQUIV:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_INEQUIV:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
    case ND_MOD:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      printf("  mov rax, rdx\n");
      break;
    case ND_AND:
      printf("  and rax, rdi\n");
      break;
    case ND_XOR:
      printf("  xor rax, rdi\n");
      break;
    case ND_OR:
      printf("  or rax, rdi\n");
      break;
    case ND_LSHIFT:
      printf("  mov rcx, rdi\n");
      printf("  sal rax, cl\n");
      break;
    case ND_RSHIFT:
      printf("  mov rcx, rdi\n");
      printf("  sar rax, cl\n");
      break;
  }
}
