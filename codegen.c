#include "9cc.h"

//////////
// static value
char x86_64_argreg[][6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
const int POINTER_SIZE_BYTES = 8;

// type helper function
int size_var(Type* ty) {
  if(ty->kind == TYPE_INT) {
    return 4;
  } else if(ty->kind == TYPE_PTR) {
    return 8;
  }
}

int size_ptr(Type* ty) {
  return size_var(ty->ptr_to);
}

// Code generator
void gen_lval(Node *node) {
  if (node->kind == ND_LVAR) {
    // save the address of lval
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset*POINTER_SIZE_BYTES);
    printf("  push rax\n");
  } else if(node->kind == ND_DEREF) {
    gen(node->children[0]);
  } else {
    error("lval is not a variable, %d", node->kind);
  }
}


void gen(Node *node) {
  int label;
  Node **stmt_list;
  Type *lhs_ty, *rhs_ty;
  LVar *var;

  switch(node->kind) {
    case ND_FUNC:
    printf("%.*s:\n", node->val, node->func_name);

    locals = node->lvars;
    // allocate stack for local variables
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", get_num_lvars() * POINTER_SIZE_BYTES);

    // copy passed argument values to the local variables
    for(int i=0;i<node->offset;++i) {
      printf("  mov [rbp-%d], %s\n", i*POINTER_SIZE_BYTES+POINTER_SIZE_BYTES, x86_64_argreg[i]);
    }


    gen(node->children[0]);
    printf("  pop rax\n");  

    // Final evaluated value is already stored on rax, which will be returned.
    // To back to the original address, we fix the rsp
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");

    return;

    case ND_RETURN:
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;

    case ND_BLOCK:
    stmt_list = node->children;
    if (*stmt_list != NULL)
      gen(*(stmt_list++));
    while (*stmt_list != NULL) {
      printf("  pop rax\n");  
      gen(*(stmt_list++));
    }
    return;

    case ND_ADDR:
    gen_lval(node->children[0]);
    return;

    case ND_DEREF:
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;

    case ND_NUM:
    printf("  push %d\n", node->val);
    return;
    
    case ND_SIZEOF:
    printf("  push %d\n", size_var(node->children[0]->ty));
    return;

    case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;

    case ND_ASSIGN:
    gen_lval(node->children[0]);
    gen(node->children[1]);
    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;

    case ND_WHILE:
    label = label_index++;
    printf(".L.begin%06d:\n", label);
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_FOR:
    label = label_index++;
    gen(node->children[0]);
    printf(".L.begin%06d:\n", label);
    gen(node->children[1]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[3]);
    gen(node->children[2]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_IF:
    label = label_index++;
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n"); // 0 if cond is not satisfied
    printf("  je .L.else%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.end%06d\n",  label);
    printf(".L.else%06d:\n",  label);
    if(node->children[2]!=NULL) {
      gen(node->children[2]);
    }
    printf(".L.end%06d:\n",  label);
    printf("  push 0\n");

    return;

    case ND_CALL:
    for(int i=0;i<node->offset;++i) {
      gen(node->children[i]);
    }

    for(int i=node->offset-1;i>=0;--i) {
      printf("  pop %s\n", x86_64_argreg[i]);
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
    printf("  push rax\n");
    return;
  }

  gen(node->children[0]);
  lhs_ty = node->children[0]->ty;
  gen(node->children[1]);
  rhs_ty = node->children[0]->ty;

  printf("  pop rdi\n");
  printf("  pop rax\n");
  
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

  printf("  push rax\n");
}
