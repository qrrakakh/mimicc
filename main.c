#include "9cc.h"

int main(int argc, char **argv) {
  Var *g;
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

  // global variable
  printf("  .data\n");
  for(g=globals;g->next!=NULL;g=g->next) {
    printf("%.*s:\n", g->len, g->name);
    printf("  .zero %d\n", size_var(g->ty));
  }

  
  return 0;
}