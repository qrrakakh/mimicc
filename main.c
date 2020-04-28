#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // tokenize and parse;
  user_input = argv[1];
  token = tokenize(user_input);
  // dummy lvar
  locals = calloc(1, sizeof(LVar)); 
  locals->next = NULL;
  program();

  // The header of assembler
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // allocate 26 variables
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", get_num_lvars() * 8); // 208 = 8 x 26

  // Follow AST and generate code
  for(int i=0;code[i];i++) {
    gen(code[i]);
    printf("  pop rax\n");
  }

  // Final evaluated value is already stored on rax, which will be returned.
  // To back to the original address, we fix the rsp
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}