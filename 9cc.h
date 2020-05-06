#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef struct Type Type;

typedef enum {
  TYPE_INT,
  TYPE_PTR,
} TypeKind;

struct Type {
  TypeKind ty;
  struct Type *ptr_to;
};

typedef struct LVar LVar;

struct LVar {  // defined local variables
  LVar *next;
  char *name;
  int len;
  int offset;
  Type *ty;
};

typedef enum {  // ABS Node kinds
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_MOD,       // %
  ND_NUM,       // integer
  ND_EQUIV,     // ==
  ND_INEQUIV,  // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_LVAR,      // Local variable,
  ND_ASSIGN,    // =
  ND_RETURN,    // return
  ND_WHILE,     // while
  ND_FOR,       // for
  ND_IF,        // if
  ND_BLOCK,     // {} block
  ND_CALL,      // function call
  ND_FUNC,      // function
  ND_ADDR,      // address
  ND_DEREF,     // dereference
} NodeKind;

typedef struct Node Node;

struct Node { // ABS Node struct
  NodeKind kind;
  Node** children;
  int val;          // Only when kind==ND_NUM or ND_CALL
  int offset;       // Only when kind=ND_LVAR or ND_CALL
  char* func_name;  // Only when kind=ND_CALL
  LVar* lvars;      // Only when kind=ND_FUNC
};

typedef enum {  // Token definition
  TK_RESERVED,  // operator symbols
  TK_IDENT,     // identifier
  TK_NUM,       // integer token
  TK_EOF,       // end of input
  TK_RETURN,    // return
} TokenKind;

typedef struct Token Token;

struct Token {  // Token type
  TokenKind kind;
  Token * next; // next input token
  int val;      // value if kind is TK_NUM
  char* str;    // token string
  int len;
};

//////////
// global variable definition
Token *token;         // current token pointer
Node *code[100];
char *user_input;     // Input program
LVar *locals;
int label_index;
Type *current_type;

//////////
// utility functions

void error(char *fmt, ...);
void error_at(char* loc, char *fmt, ...);

//////////
// token-related functions
Token *tokenize(char *p);

//////////
// ast-related functions

// find if the local var is already defined
LVar *find_lvar_by_offset(int offset);

// get a number of local variables
int get_num_lvars();

// Non-terminal symbols generator
void program();

// Code generator
void gen(Node *node);

//////////
// debug function
void print_node_tree();
