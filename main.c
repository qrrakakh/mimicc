#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // tokenize and parse;
  user_input = argv[1];
  token = tokenize(user_input);

  // init label index
  label_index = 0;

  program();
  //print_node_tree();

  // The header of assembler
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");

  // Follow AST and generate code
  for(int i=0;code[i];i++) {
    gen(code[i]);
    //printf("  pop rax\n");
  }

  return 0;
}