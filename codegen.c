#include "9cc.h"

//////////
// static value
char x86_64_argreg_8bits[][6]  = {"dil", "sil", "dl",  "cl",  "r8b", "r9b"};
char x86_64_argreg_16bits[][6] = {"di",  "si",  "dx",  "cx",  "r8w", "r9w"};
char x86_64_argreg_32bits[][6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
char x86_64_argreg_64bits[][6] = {"rdi", "rsi", "rdx", "rcx", "r8",  "r9" };
const int POINTER_SIZE_BYTES = 8;

// type helper function
int size_var(Type* ty) {
  if(ty->kind == TYPE_CHAR) {
    return 1;
  } else if(ty->kind == TYPE_INT) {
    return 4;
  } else if(ty->kind == TYPE_PTR) {
    return 8;
  } else if(ty->kind == TYPE_ARRAY) {
    return ty->array_size * size_var(ty->ptr_to);
  }
}

int size_ptr(Type* ty) {
  return size_var(ty->ptr_to);
}

// Code generator
void store(Type *ty) {
  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch(size_var(ty)) {
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

  printf("  mov rax, rdi\n");
}

void load(Type *ty) {
  switch(size_var(ty)) {
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

void gen_lval(Node *node) {
  Var *var;
  if (node->kind == ND_LVAR) {
    // save the address of lval
    var = find_lvar_by_id(node->id);
    printf("  lea rax, [rbp-%d]\n", var->offset_bytes);
  } else if(node->kind == ND_GVAR) {
    var = find_gvar_by_id(node->id);
    printf("  lea rax, %.*s[rip]\n", var->len, var->name);
  } else if(node->kind == ND_DEREF) {
    gen(node->children[0]);
  } else {
    error("lval is not a variable, %d", node->kind);
  }
}


void gen(Node *node) {
  int label;
  int lvar_area_size;
  int num_lvar;
  int lvar_idx;
  Node **stmt_list;
  Type *lhs_ty, *rhs_ty;
  Var *var;

  switch(node->kind) {
    case ND_FUNC:
    printf("%.*s:\n", node->val, node->func_name);

    locals = node->lvars;
    // allocate stack for local variables
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");

    // calculate required reserved stack area sizeand offsets for each lvars
    lvar_area_size = 0;
    num_lvar = 0;
    var = locals;
    while (var->next) {
      ++num_lvar;
      lvar_area_size += size_var(var->ty);
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
      switch(size_var(var->ty)) {
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
        printf("  mov [rbp-%d], %s\n", var->offset_bytes, x86_64_argreg_8bits[lvar_idx]);
        break;
      }
      var = var->next;
    }


    gen(node->children[0]);

    // Final evaluated value is already stored on rax, which will be returned.
    // To back to the original address, we fix the rsp
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");

    return;

    case ND_RETURN:
    gen(node->children[0]);
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;

    case ND_BLOCK:
    stmt_list = node->children;
    if (*stmt_list != NULL)
      gen(*(stmt_list++));
    while (*stmt_list != NULL) {
      gen(*(stmt_list++));
    }
    return;

    case ND_ADDR:
    gen_lval(node->children[0]);
    return;

    case ND_DEREF:
    gen(node->children[0]);
    printf("  mov rax, [rax]\n");
    return;

    case ND_NUM:
    case ND_CHAR:
    printf("  mov rax, %d\n", node->val);
    return;

    case ND_SIZEOF:
    // TODO: when we implement struct, judge if the var is struct or not
    printf("  mov rax,%d\n", size_var(node->children[0]->ty));
    return;

    case ND_GVAR:
    if (node->val==1)
      return;
    case ND_LVAR:
    gen_lval(node);
    load(node->ty);
    return;

    case ND_ASSIGN:
    gen_lval(node->children[0]);
    printf("  push rax\n");
    gen(node->children[1]);
    printf("  push rax\n");
    store(node->ty);
    return;

    case ND_WHILE:
    label = label_index++;
    printf(".L.begin%06d:\n", label);
    gen(node->children[0]);
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    return;

    case ND_FOR:
    label = label_index++;
    gen(node->children[0]);
    printf(".L.begin%06d:\n", label);
    gen(node->children[1]);
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[3]);
    gen(node->children[2]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    return;

    case ND_IF:
    label = label_index++;
    gen(node->children[0]);
    printf("  cmp rax, 0\n"); // 0 if cond is not satisfied
    printf("  je .L.else%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.end%06d\n",  label);
    printf(".L.else%06d:\n",  label);
    if(node->children[2]!=NULL) {
      gen(node->children[2]);
    }
    printf(".L.end%06d:\n",  label);
    return;

    case ND_CALL:
    for(int i=0;i<node->num_args;++i) {
      gen(node->children[i]);
      switch(size_var(node->children[i]->ty)) {
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
    printf("  call %.*s\n", node->val, node->func_name);
    printf("  jmp .L.end%06d\n", label);
    printf(".L.call%06d:\n", label);
    printf("  sub rsp, 8\n");
    printf("  mov rax, 0\n");
    printf("  call %.*s\n", node->val, node->func_name);
    printf("  add rsp, 8\n");
    printf(".L.end%06d:\n", label);
    return;
  }

  gen(node->children[1]);
  rhs_ty = node->children[0]->ty;
  printf("  push rax\n");
  gen(node->children[0]);
  lhs_ty = node->children[0]->ty;
  printf("  pop rdi\n");
  
  switch(node->kind) {
    case ND_ADD:
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",size_ptr(lhs_ty));
      } else if(rhs_ty->kind == TYPE_PTR) {
        printf("  imul rax, %d\n",size_ptr(rhs_ty));
      }
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:    
      if(lhs_ty->kind == TYPE_PTR) {
        printf("  imul rdi, %d\n",size_ptr(lhs_ty));
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
  }
}
