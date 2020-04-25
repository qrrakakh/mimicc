#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // tokenize and parse;
  user_input = argv[1];
  token = tokenize(user_input);
  Node *node = expr();

  // The header of assembler
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Follow AST and generate code
  gen(node);

  // Finally load the top value of the stack
  // and return this.
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}