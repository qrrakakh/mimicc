#include "9cc.h"

// Code generator
void gen_lval(Node* node) {
  if (node->kind != ND_LVAR) {
    error("lval is not a variable");
  }
  // save the address of lval
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}


void gen(Node *node) {
  int label;
  Node **stmt_list;
  switch(node->kind) {
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

    case ND_NUM:
    printf("  push %d\n", node->val);
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
    printf(".Lbegin%06d:\n", label);
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .Lbegin%06d\n", label);
    printf(".Lend%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_FOR:
    label = label_index++;
    gen(node->children[0]);
    printf(".Lbegin%06d:\n", label);
    gen(node->children[1]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%06d\n", label);
    gen(node->children[3]);
    gen(node->children[2]);
    printf("  jmp .Lbegin%06d\n", label);
    printf(".Lend%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_IF:
    label = label_index++;
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n"); // 0 if cond is not satisfied
    printf("  je .Lelse%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .Lend%06d\n",  label);
    printf(".Lelse%06d:\n",  label);
    if(node->children[2]!=NULL) {
      gen(node->children[2]);
    }
    printf(".Lend%06d:\n",  label);
    printf("  push 0\n");

    return;

    case ND_CALL:
    printf("  call %.*s\n", node->val, node->func_name);
    printf("  push rax\n");
    return;
  }

  gen(node->children[0]);
  gen(node->children[1]);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch(node->kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
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
  }

  printf("  push rax\n");
}
