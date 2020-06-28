#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "mimicc.h"

void Compile(char *input) {
  // tokenize and parse;
  token = Tokenize(input);

  // init label index
  label_index = 0;

  // init built-in type
  builtin_type_obj = calloc(num_builtin_types, sizeof(Type*));
  for(int i=0;i<num_builtin_types;++i) {
    builtin_type_obj[i] = calloc(1, sizeof(Type));
    builtin_type_obj[i]->kind = builtin_type_enum[i];
    builtin_type_obj[i]->ptr_to = NULL;
  }

  int_type = builtin_type_obj[0];
  char_type = builtin_type_obj[1];
  void_type = builtin_type_obj[2];
  bool_type = builtin_type_obj[3];

  program();
  //PrintNodeTree();

  // The header of assembler
  InitProgram();

  // Follow AST and generate code
  for(int i=0;codes[i];i++) {
    Generate(codes[i]);
  }

  GenerateFooter();
}

char *ReadFile(char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) {
    Error("cannot open *s: *s", path, strerror(errno));
  }

  if (fseek(fp, 0, SEEK_END) == -1)
    Error("%s: fseek: %s", path, strerror(errno));
  size_t size = ftell(fp);
  if (fseek(fp, 0, SEEK_SET) == -1)
    Error("%s: fseek: %s", path, strerror(errno));
  
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
    file_path = argv[i];
    user_input = ReadFile(file_path);
    Compile(user_input);
  }

  return 0;
}