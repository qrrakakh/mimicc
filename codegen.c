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
  switch(node->kind) {
    case ND_RETURN:
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
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
    printf(".Lbegin%03d:\n", 0);
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%03d\n", 0);
    gen(node->children[1]);
    printf("  jmp .Lbegin%03d\n", 0);
    printf(".Lend%03d:\n", 0);
    printf("  push 0\n");
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
