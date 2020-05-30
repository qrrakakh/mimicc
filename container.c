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
  char *line = loc;
  while (user_input < line && line[-1] != '\n')
    --line;
  
  char *end = loc;
  while (*end != '\n')
    end++;
  
  int line_num = 1;
  for(char *p = user_input;p<line;p++)
    if(*p == '\n')
      line_num++;
  
  int indent = fprintf(stderr, "%s:%d: ", filepath, line_num);
  fprintf(stderr, "%.*s\n", (int)(end - line), line);
  int pos = loc - line + indent;
  fprintf(stderr, "%*s", pos, "");

  va_list ap;
  va_start(ap, fmt);

  fprintf(stderr, "^");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

// warn reporter with failed position, but continue to compile.
void warn_at(char *loc, char *fmt, ...) {
  char *line = loc;
  while (user_input < line && line[-1] != '\n')
    --line;
  
  char *end = loc;
  while (*end != '\n')
    end++;
  
  int line_num = 1;
  for(char *p = user_input;p<line;p++)
    if(*p == '\n')
      line_num++;
  
  int indent = fprintf(stderr, "%s:%d: ", filepath, line_num);
  fprintf(stderr, "%.*s\n", (int)(end - line), line);
  int pos = loc - line + indent;
  fprintf(stderr, "%*s", pos, "");

  va_list ap;
  va_start(ap, fmt);

  fprintf(stderr, "^");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

// debug function
void print_node(Node* node, int num_lead_ws) {
  fprintf(stderr, "%*s---\n",num_lead_ws*4, "");
  fprintf(stderr, "%*skind: %d\n",num_lead_ws*4,"", node->kind);
  fprintf(stderr, "%*sval: %d\n",num_lead_ws*4,"",node->val);
  fprintf(stderr, "%*sid: %d\n",num_lead_ws*4,"",node->id);
  fprintf(stderr, "%*snum_args: %d\n",num_lead_ws*4,"",node->num_args);
  fprintf(stderr, "%*soffset: %d\n",num_lead_ws*4,"",node->offset);
  if(node->ty) {
    fprintf(stderr, "%*stype kind: %d\n",num_lead_ws*4,"", node->ty->kind);
  }
  fprintf(stderr, "%*sfunc_name: %.*s\n",num_lead_ws*4,"",node->val, node->func_name);
  int num_children = 0;
  if (node->kind==ND_CALL) {
    num_children = node->num_args;
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
    print_node(*(node->children+i), num_lead_ws+1);
  }
}

void print_node_tree() {
  for(int i=0;code[i];i++) {
    print_node(code[i], 0);
  }
}

void print_lvar() {
  Var* l = locals;
  while(l->next) {
    printf("%.*s, id: %d, type: %d\n", l->len, l->name, l->id, l->ty->kind);
    l=l->next;
  }
}