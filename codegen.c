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
  if(ty->kind == TYPE_CHAR || ty->kind == TYPE_BOOL || ty->kind == TYPE_VOID) {
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

int GetVarSize(Type *ty) {
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

int GetTargetPtrSize(Type *ty) {
  return GetTypeSize(ty->ptr_to);
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
// Code generate helper
void StoreVar(Type *ty, int offset, _Bool eval) {
  printf("  pop rdi\n");
  printf("  pop rax\n");

  if(offset>0) {
    switch(GetVarSize(ty)) {
      case 1:
        printf("  mov [rax+%d], dil\n", offset);
        break;
      case 2:
        printf("  mov [rax+%d], di\n", offset);
        break;
      case 4:
        printf("  mov [rax+%d], edi\n", offset);
        break;
      case 8:
        printf("  mov [rax+%d], rdi\n", offset);
        break;
    }
  } else {
    switch(GetVarSize(ty)) {
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
  }

  if (eval) {
    printf("  mov rax, rdi\n");
  }
}

void LoadVar(Type *ty) {
  if(ty->kind == TYPE_ARRAY)
    return;
  switch(GetVarSize(ty)) {
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

//////////
// Initializer
void CstrData(Const_Strings *cstr) {
  _Bool is_escaped = 0;
  char c;
  for(int i=0;i<cstr->tok->len;++i) {
    c = (cstr->tok->str)[i];
    if (is_escaped) {
      printf("  .byte %#x\n", GetEscapedChar(c));
      is_escaped = 0;
    } else if(c == '\\') {
      is_escaped = 1;
    } else {
      printf("  .byte %#x\n", c);
    }
  }
  printf("  .byte 0\n");
}

void InitLvar(Node *node, int offset, _Bool eval);

void InitOrdinaryLvar(Node *node, int offset, _Bool eval) {
  // assume that the head address is stored in rax.
  printf("  push rax\n");
  Generate(node->children[0]);
  printf("  push rax\n");
  StoreVar(node->ty, offset, eval);
}

void InitArrayLvar(Node *node, int offset) {
  // assume that the head address is stored in rax.
  Node *node_cur = node;
  for(int i=0;i<node->ty->array_size;++i) {
    printf("  push rax\n");
    InitLvar(node_cur->children[0], i*GetTargetPtrSize(node->ty)+offset,0);
    printf("  pop rax\n");

    node_cur = node_cur->children[1];
    if(node_cur->num_args == 0) {
      break;
    }
  }
}

void InitCstrLvar(Node *node, int offset) {
  Const_Strings *cstr = FindCstrById(node->children[0]->id);
  _Bool is_escaped = 0;
  char c;
  int j=0;
  for(int i=0;i<cstr->tok->len;++i) {
    c = (cstr->tok->str)[i];
    if (is_escaped) {
      printf("  mov byte ptr [rax+%d], %#x\n", (j++)+offset, GetEscapedChar(c));
      is_escaped = 0;
    } else if(c == '\\') {
      is_escaped = 1;
    } else {
      printf("  mov byte ptr [rax+%d], %#x\n", (j++)+offset, c);
    }
  }
  printf("  mov byte ptr [rax+%d], 0\n", j+offset);
}

void InitLvar(Node *node, int offset, _Bool eval) {
  // assume that the head address is stored in rax.
  if(node->ty->kind == TYPE_ARRAY) {
    if (node->ty->ptr_to->kind == TYPE_CHAR && node->num_args == 1) {
      InitCstrLvar(node, offset);
    } else {
      InitArrayLvar(node, offset);
    }
  } else if(node->ty->kind == TYPE_STRUCT) {
    Error("struct initialization is not supported.");
  } else {
    InitOrdinaryLvar(node, offset, eval);
  }
}

void InitGvar(Node *node);

void InitOrdinaryGvar(Node *node) {
  int type_size = GetTypeSize(node->ty);
  if(type_size==1) {
    printf("  .byte %d\n", Eval(node->children[0]));
  } else {
    printf("  .%dbyte %d\n", type_size, Eval(node->children[0]));
  }
}

void InitArrayGvar(Node *node) {
  Node *node_cur = node;
  for(int i=0;i<node->ty->array_size;++i) {
    InitGvar(node_cur->children[0]);
    node_cur = node_cur->children[1];
    if(node_cur->num_args == 0 && (node->ty->array_size - (i+1) > 0)) {
      printf("  .zero %ld\n", GetTypeSize(node->ty->ptr_to) * (node->ty->array_size - (i+1)));
      break;
    }
  }
}

void InitCstrGvar(Node *node) {
  Const_Strings *cstr = FindCstrById(node->children[0]->id);
  CstrData(cstr);
  int num_zeros = node->ty->array_size - cstr->tok->len-1;
  if(num_zeros>0) {
    printf("  .zero %d\n", num_zeros);
  }
}

Token *GetTargetGVarName(Node *node) {
  if(node->kind == ND_ADDR) {
      if(node->children[0]->kind != ND_GVAR) {
        ErrorAt(node->children[0]->tok->str, "global variable is expected.");
      }
      return node->children[0]->tok;
  } else if(node->kind == ND_GVAR) {
    if(node->ty->kind != TYPE_ARRAY) {
      ErrorAt(node->tok->str, "array global variable is expected.");
    }
    return node->tok;
  } else {
    ErrorAt(node->tok->str, "invalid initializer for global variable.");
    return NULL;
  }
}

void InitPtrGvar(Node *node) {
  Token *var_token;
  int val;
  switch(node->children[0]->kind) {
    case ND_STRINGS:
      printf("  .quad .LC%06d\n", node->children[0]->id);
      return;
    case ND_ADD:
      var_token = GetTargetGVarName(node->children[0]->children[0]);
      val = Eval(node->children[0]->children[1]);
      printf("  .quad %.*s+%d\n", var_token->len, var_token->str , val*GetTargetPtrSize(node->ty));
      return;
    case ND_SUB:
      var_token = GetTargetGVarName(node->children[0]->children[0]);
      val = Eval(node->children[0]->children[1]);
      printf("  .quad %.*s-%d\n", var_token->len, var_token->str , val*GetTargetPtrSize(node->ty));
      return;
    default:
      var_token = GetTargetGVarName(node->children[0]);
      printf("  .quad %.*s\n", var_token->len, var_token->str);
  }
}

void InitGvar(Node *node) {
  if(node->ty->kind == TYPE_ARRAY) {
    if (node->ty->ptr_to->kind == TYPE_CHAR && node->num_args == 1) {
      InitCstrGvar(node);
    } else {
      InitArrayGvar(node);
    }
  } else if(node->ty->kind == TYPE_PTR) {
    InitPtrGvar(node);
  } else if(node->ty->kind == TYPE_STRUCT) {
    Error("struct initialization is not supported.");
  } else {
    InitOrdinaryGvar(node);
  }
}

//////////
// Code generator
void InitProgram() {
  // calculate size and offsets of each structs
  Struct *s = structs;
  Symbol *v;
  int member_size, alignment, diff, max_align;

  while(s->next) {
    if(!(s->members)) {
      s = s->next;
      continue;
    }
    // get the tail of members list; begin from the initially declared member
    for(v=s->members;v->next;v=v->next);

    s->size = 0;
    max_align = 1;
    for(v=v->prev;v;v=v->prev) {
      v->offset_bytes = s->size;
      if(v->ty->kind == TYPE_ARRAY) {
        alignment = GetTypeSize(v->ty->ptr_to);
        member_size = alignment * v->ty->array_size;
      } else {
        member_size = GetTypeSize(v->ty);
        alignment = member_size;
      }
      s->size += member_size;
      diff = v->offset_bytes % alignment;
      if (diff>0) {
        v->offset_bytes += alignment - diff;
      }
      s->size = v->offset_bytes + member_size;
      max_align = (alignment > max_align)? alignment: max_align;
    }

    if (s->size % max_align > 0) {
      s->size = (s->size / max_align + 1) * max_align;
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
    if(g->ty->tq & TQ_CONST) { // const
      continue;
    }
    printf("  .global %.*s\n", g->len, g->name);
    printf("%.*s:\n", g->len, g->name);
    if(g->initializer) {
      InitGvar(g->initializer);
    } else {
      if (g->ty->kind == TYPE_STRUCT) {
        printf("  .zero %d\n", GetTypeSize(g->ty));
      } else if (g->ty->kind == TYPE_ARRAY) {
        printf("  .zero %ld\n", GetTypeSize(g->ty->ptr_to) * g->ty->array_size);
      } else {
        printf("  .zero %d\n", GetTypeSize(g->ty));
      }
    }
  }

  // read-only data
  printf("  .section .rodata\n");
  //// const gloval variable
  for(g=globals;g->next!=NULL;g=g->next) {
    if(g->scope_id==-1) { // extern
      continue;
    }
    if(g->kind!=SY_VAR) { // not a variable
      continue;
    }
    if(!(g->ty->tq & TQ_CONST)) { // not const
      continue;
    }
    printf("  .global %.*s\n", g->len, g->name);
    printf("%.*s:\n", g->len, g->name);
    if(g->initializer) {
      InitGvar(g->initializer);
    } else {
      if (g->ty->kind == TYPE_STRUCT) {
        printf("  .zero %d\n", GetTypeSize(g->ty));
      } else if (g->ty->kind == TYPE_ARRAY) {
        printf("  .zero %ld\n", GetTypeSize(g->ty->ptr_to) * g->ty->array_size);
      } else {
        printf("  .zero %d\n", GetTypeSize(g->ty));
      }
    }
  }

  //// String literal
  for(c=cstrs;c->next!=NULL;c=c->next) {
    printf(".LC%06d:\n", c->id);
    CstrData(c);
  }
  printf("\n");
}


void Generate(Node *node) {
  int label;
  int lvar_area_size;
  int num_lvar;
  int arg_idx;
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
      lvar_area_size = current_func->has_varargs? 56: 0;
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

      lvar_area_size =  (lvar_area_size  + 16 - 1) / 16 * 16;
      
      printf("  sub rsp, %d\n", lvar_area_size);

      // save register if the function has var args
      if(current_func->has_varargs) {
        printf("  mov dword ptr [rbp-8], %d\n", node->num_args * 8);
        printf("  mov [rbp-16], r9\n");
        printf("  mov [rbp-24], r8\n");
        printf("  mov [rbp-32], rcx\n");
        printf("  mov [rbp-40], rdx\n");
        printf("  mov [rbp-48], rsi\n");
        printf("  mov [rbp-56], rdi\n");
      }

      // copy passed argument values to the local variables
      var = locals;
      while(num_lvar > node->num_args) {
        if(var->kind != SY_VAR) {
          var = var->next;
          continue;
        }
        var=var->next; --num_lvar;
      }

      for(int i=0;i<node->num_args;++i) {
        arg_idx = node->num_args-1-i;
        switch(GetVarSize(var->ty)) {
          case 1:
            printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_8bits[arg_idx]);
            break;
          case 2:
            printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_16bits[arg_idx]);
            break;
          case 4:
            printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_32bits[arg_idx]);
            break;
          case 8:
            printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_64bits[arg_idx]);
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
      if (node->children[0]) {
        printf("  mov rax,%d\n", GetTypeSize(node->children[0]->ty));
      } else {
        printf("  mov rax,%d\n", GetTypeSize(node->ty->ptr_to));
      }
      return;

    case ND_NOT:
      Generate(node->children[0]);
      printf("  not eax\n");
      return;

    case ND_LNOT:
      Generate(node->children[0]);
      printf("  cmp rax, 0\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      return;

    case ND_PREINC:
      diff = 1;
      if(node->children[0]->ty->kind == TYPE_PTR) {
        diff = GetTargetPtrSize(node->children[0]->ty);
      }
      GenLval(node->children[0]);
      printf("  push rax\n");
      LoadVar(node->children[0]->ty);
      printf("  add rax, %d\n", diff);
      printf("  push rax\n");
      StoreVar(node->children[0]->ty, 0, 1);
      return;

    case ND_PREDEC:
      diff = 1;
      if(node->children[0]->ty->kind == TYPE_PTR) {
        diff = GetTargetPtrSize(node->children[0]->ty);
      }
      GenLval(node->children[0]);
      printf("  push rax\n");
      LoadVar(node->children[0]->ty);
      printf("  sub rax, %d\n", diff);
      printf("  push rax\n");
      StoreVar(node->children[0]->ty, 0, 1);
      return;

    case ND_POSTINC:
      diff = 1;
      if(node->children[0]->ty->kind == TYPE_PTR) {
        diff = GetTargetPtrSize(node->children[0]->ty);
      }
      GenLval(node->children[0]);
      printf("  push rax\n");
      LoadVar(node->children[0]->ty);
      printf("  mov rsi, rax\n");
      printf("  add rax, %d\n", diff);
      printf("  push rax\n");
      StoreVar(node->children[0]->ty, 0, 0);
      printf("  mov rax, rsi\n");
      return;

    case ND_POSTDEC:
      diff = 1;
      if(node->children[0]->ty->kind == TYPE_PTR) {
        diff = GetTargetPtrSize(node->children[0]->ty);
      }
      GenLval(node->children[0]);
      printf("  push rax\n");
      LoadVar(node->children[0]->ty);
      printf("  mov rsi, rax\n");
      printf("  sub rax, %d\n", diff);
      printf("  push rax\n");
      StoreVar(node->children[0]->ty, 0, 0);
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
      InitLvar(node, 0, 1);
      return;

    case ND_ASSIGN:
      if(node->children[0]->ty->tq & TQ_CONST) {
        ErrorAt(node->children[0]->tok->str, "assignment of read-only location.");
      }
      GenLval(node->children[0]);
      printf("  push rax\n");
      Generate(node->children[1]);
      printf("  push rax\n");
      StoreVar(node->ty, 0, 1);
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
    case ND_CONDEXPR:
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
      // built-in function: va_start
      if(node->val == 8 && !memcmp(node->name, "va_start", 8)) {
        Generate(node->children[0]);

        // gp_offset
        printf("  movsxd rdi, dword ptr [rbp-8]\n");
        printf("  mov dword ptr [rax], edi\n");

        // fp_offset (fp not supported, fixed value: 8 x 6 = 48)
        printf("  mov dword ptr [rax+4], 48\n");

        // overflow_arg_area (argument more than 6 is not supported, fixed value: NULL)
        printf("  mov qword ptr [rax+8], 0\n");

        // reg_save_area: rbp - 8 x (6+1) = rbp-56
        printf("  mov [rax+16], rbp\n");
        printf("  sub qword ptr [rax+16], 56\n");
        return;
      }

      for(int i=0;i<node->num_args;++i) {
        Generate(node->children[i]);
        printf("  push rax\n");
      }

      for(int i=node->num_args-1;i>=0;--i) {
        printf("  pop %s\n", x86_64_argreg_64bits[i]);
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

  switch(node->kind) {
    case ND_LAND:
      label = label_index++;
      Generate(node->children[0]);
      printf("  cmp rax, 0\n");
      printf("  je .L.false%06d\n", label);
      Generate(node->children[1]);
      printf("  cmp rax, 0\n");
      printf("  je .L.false%06d\n", label);
      printf("  mov rax, 1\n");
      printf("  jmp .L.end%06d\n", label);
      printf(".L.false%06d:\n", label);
      printf("  mov rax, 0\n");
      printf(".L.end%06d:\n", label);
      return;
    case ND_LOR:
      label = label_index++;
      Generate(node->children[0]);
      printf("  cmp rax, 0\n");
      printf("  jnz .L.true%06d\n", label);
      Generate(node->children[1]);
      printf("  cmp rax, 0\n");
      printf("  jnz .L.true%06d\n", label);
      printf("  mov rax, 0\n");
      printf("  jmp .L.end%06d\n", label);
      printf(".L.true%06d:\n", label);
      printf("  mov rax, 1\n");
      printf(".L.end%06d:\n", label);
      return;
  }

  Generate(node->children[1]);
  rhs_ty = node->children[1]->ty;
  printf("  push rax\n");
  Generate(node->children[0]);
  lhs_ty = node->children[0]->ty;
  printf("  pop rdi\n");
  
  switch(node->kind) {
    case ND_ADD:
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",GetTargetPtrSize(lhs_ty));
      } else if(rhs_ty->kind == TYPE_PTR) {
        printf("  imul rax, %d\n",GetTargetPtrSize(rhs_ty));
      }
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:    
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",GetTargetPtrSize(lhs_ty));
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
