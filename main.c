#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "mimicc.h"

void compile(char *input) {
  // tokenize and parse;
  token = tokenize(input);

  // init label index
  label_index = 0;

  program();
  //print_node_tree();

  // The header of assembler
  gen_header();

  // Follow AST and generate code
  for(int i=0;code[i];i++) {
    gen(code[i]);
  }

  gen_footer();
}

char *read_file(char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) {
    error("cannot open *s: *s", path, strerror(errno));
  }

  if (fseek(fp, 0, SEEK_END) == -1)
    error("%s: fseek: %s", path, strerror(errno));
  size_t size = ftell(fp);
  if (fseek(fp, 0, SEEK_SET) == -1)
    error("%s: fseek: %s", path, strerror(errno));
  
  char *buff = calloc(1, size+2);
  fread(buff, size, 1, fp);

  if (size == 0 || buff[size-1] != '\n')
    buff[size++] = '\n';
  
  buff[size] = '\0';
  fclose(fp);
  return buff;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  for(int i=1;i<argc;++i) {
    filepath = argv[i];
    user_input = read_file(filepath);
    compile(user_input);
  }

  return 0;
}