#include "9cc.h"

//////////
// utility functions

// error reporter which takes same args to printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

// error reporter with failed position
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

// debug function
void print_node(Node* node, int offset) {
  fprintf(stderr, "%*s---\n",offset*4, "");
  fprintf(stderr, "%*skind: %d\n",offset*4,"", node->kind);
  fprintf(stderr, "%*sval: %d\n",offset*4,"",node->val);
  fprintf(stderr, "%*soffset: %d\n",offset*4,"",node->offset);
  if(node->ty) {
    fprintf(stderr, "%*stype kind: %d\n",offset*4,"", node->ty->kind);
  }
  fprintf(stderr, "%*sfunc_name: %s\n",offset*4,"",node->func_name);
  int num_children = 0;
  if (node->kind==ND_CALL) {
    num_children = node->offset;
  } else if(node->kind==ND_RETURN || node->kind == ND_SIZEOF
          || node->kind == ND_ADDR || node->kind == ND_DEREF
          || node->kind == ND_FUNC) {
    // unary op / func
    num_children = 1;
  }  else if(node->kind==ND_WHILE || node->kind == ND_ASSIGN || node->kind == ND_EQUIV || node->kind == ND_INEQUIV
            || node->kind == ND_LE || node->kind == ND_LT
            || node->kind == ND_ADD || node->kind == ND_SUB || node->kind == ND_MUL || node->kind == ND_DIV
            || node->kind == ND_MOD ) {
    // bin op
    num_children = 2;
  }  else if(node->kind==ND_IF) {
    num_children = 3;
    if(!(*(node->children+2))) {
      num_children = 2;
    }
  }  else if(node->kind==ND_FOR) {
    num_children = 4;
  } else if(node->kind==ND_BLOCK) {
    int i;
    for(i=0;node->children[i];i++){;}
    num_children = i;
  }

  for(int i=0;i<num_children;++i) {
    print_node(*(node->children+i), offset+1);
  }
}

void print_node_tree() {
  for(int i=0;code[i];i++) {
    print_node(code[i], 0);
  }
}
