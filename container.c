#include "mimicc.h"

//////////
// utility functions

// error reporter which takes same args to printf
void Error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

// error reporter with failed position
void ErrorAt(char *loc, char *fmt, ...) {
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
  
  int indent = fprintf(stderr, "%s:%d: ", file_path, line_num);
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
void WarnAt(char *loc, char *fmt, ...) {
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
  
  int indent = fprintf(stderr, "%s:%d: ", file_path, line_num);
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
void PrintNode(Node *node, int num_lead_ws) {
  fprintf(stderr, "%*s---\n",num_lead_ws*4, "");
  fprintf(stderr, "%*skind: %d\n",num_lead_ws*4,"", node->kind);
  fprintf(stderr, "%*sval: %d\n",num_lead_ws*4,"",node->val);
  fprintf(stderr, "%*sid: %d\n",num_lead_ws*4,"",node->id);
  fprintf(stderr, "%*snum_args: %d\n",num_lead_ws*4,"",node->num_args);
  fprintf(stderr, "%*soffset: %d\n",num_lead_ws*4,"",node->offset);
  if(node->ty) {
    fprintf(stderr, "%*stype kind: %d\n",num_lead_ws*4,"", node->ty->kind);
  }
  fprintf(stderr, "%*sfunc_name: %.*s\n",num_lead_ws*4,"",node->val, node->name);
  int num_children = 0;
  if (node->kind==ND_CALL) {
    num_children = node->num_args;
  } else if(node->kind==ND_RETURN || node->kind == ND_SIZEOF
          || node->kind == ND_ADDR || node->kind == ND_DEREF
          || node->kind == ND_NOT
          || node->kind == ND_FUNC
          || node->kind == ND_ARROW || node->kind == ND_DOT) {
    // unary op / func
    num_children = 1;
  }  else if(node->kind==ND_WHILE || node->kind == ND_ASSIGN || node->kind == ND_EQUIV || node->kind == ND_INEQUIV
            || node->kind == ND_LE || node->kind == ND_LT
            || node->kind == ND_AND || node->kind == ND_XOR || node->kind == ND_OR
            || node->kind == ND_ADD || node->kind == ND_SUB || node->kind == ND_MUL || node->kind == ND_DIV
            || node->kind == ND_MOD) {
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
  } else if(node->kind==ND_LIST) {
    num_children = node->num_args;
  }

  for(int i=0;i<num_children;++i) {
    if(*(node->children+i)==NULL) {
      fprintf(stderr, "%*s---\n",num_lead_ws*4+4, "");
      fprintf(stderr, "%*s<NULL NODE>\n", num_lead_ws*4+4,"");
  } else {
      PrintNode(*(node->children+i), num_lead_ws+1);
    }
  }
}

void PrintNodeTree() {
  for(int i=0;codes[i];i++) {
    PrintNode(codes[i], 0);
  }
}

void PrintLvar() {
  Symbol *l = locals;
  while(l->next) {
    printf("%.*s, id: %d, type: %d\n", l->len, l->name, l->id, l->ty->kind);
    l=l->next;
  }
}

char GetEscapedChar(char x) {
  switch(x) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      return x-'0';
    case 'a':
      return 7;
    case 'b':
      return 8;
    case 'e':
    case 'E':
      return 27;
    case 'f':
      return 12;
    case 'n':
      return 10;
    case 'r':
      return 13;
    case 't':
      return 9;
    case 'v':
      return 11;
    default: // including ', ", ?, backslash
      return x;
  }
}

//////////
// evaluator for constant

int Eval(Node *node) {
  switch(node->kind) {
    case ND_ASSIGN:
      ErrorAt(node->tok->str, "Assignment is not a constant.");
    case ND_OR:
      return Eval(node->children[0]) | Eval(node->children[1]);
    case ND_XOR:
      return Eval(node->children[0]) ^ Eval(node->children[1]);
    case ND_AND:
      return Eval(node->children[0]) & Eval(node->children[1]);
    case ND_EQUIV:
      return Eval(node->children[0]) == Eval(node->children[1]);
    case ND_INEQUIV:
      return Eval(node->children[0]) != Eval(node->children[1]);
    case ND_LT:
      return Eval(node->children[0]) < Eval(node->children[1]);
    case ND_LE:
      return Eval(node->children[0]) <= Eval(node->children[1]);
    case ND_LSHIFT:
      return Eval(node->children[0]) << Eval(node->children[1]);
    case ND_RSHIFT:
      return Eval(node->children[0]) >> Eval(node->children[1]);
    case ND_MUL:
      return Eval(node->children[0]) * Eval(node->children[1]);
    case ND_DIV:
      return Eval(node->children[0]) / Eval(node->children[1]);
    case ND_MOD:
      return Eval(node->children[0]) % Eval(node->children[1]);
    case ND_ADD:
      return Eval(node->children[0]) + Eval(node->children[1]);
    case ND_SUB:
      return Eval(node->children[0]) - Eval(node->children[1]);
    case ND_NUM:
    case ND_CHAR:
      return node->val;
    default:
      break;
  }
  ErrorAt(node->tok->str, "Not a constant.");
  return 0;
}

//////////
// global variable definition
Token *token;         // current token pointer
Node **codes;
char *file_path;     // Input program path
char *user_input;     // Input program
Symbol *locals;
Symbol *globals;
Func *funcs;
Func *current_func;
Struct *structs;
Enum *enums;
int last_symbol_id;
int last_struct_id;
int last_enum_id;
Node *current_switch;
Const_Strings *cstrs;
int label_index;
int last_scope_id;
int ctrl_depth;
Scope *current_scope;