#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "mimicc.h"

void Compile(char *input) {
  // tokenize and parse;
  token = Tokenize(input);

  // init label index
  label_index = 0;

  translation_unit();
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