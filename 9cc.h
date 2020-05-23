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
  TYPE_CHAR,
  TYPE_INT,
  TYPE_ARITHMETIC_LIMIT,
  TYPE_PTR,
  TYPE_ARRAY,
} TypeKind;

struct Type {
  TypeKind kind;
  struct Type *ptr_to;
  size_t array_size;
};

typedef struct Var Var;

struct Var {  // defined variables
  Var *next;
  char *name;
  int len;
  int id;
  int offset_bytes;
  Type *ty;
};

typedef enum {  // ABS Node kinds
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_MOD,       // %
  ND_NUM,       // integer
  ND_CHAR,      // char
  ND_EQUIV,     // ==
  ND_INEQUIV,  // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_LVAR,      // Local variable
  ND_GVAR,      // Global variable
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
  ND_SIZEOF,    // sizeof
} NodeKind;

typedef struct Node Node;

struct Node { // ABS Node struct
  NodeKind kind;
  Node** children;
  int val;          // Only when kind==ND_NUM or ND_CALL or ND_GVAR
  int offset;       // Only when kind=ND_LVAR or ND_GVAR
  int id;           // Only when kind=ND_LVAR or ND_GVAR
  int num_args;     // Only when kind=ND_FUNC or ND_CALL
  char* func_name;  // Only when kind=ND_CALL
  Var* lvars;      // Only when kind=ND_FUNC
  Type* ty;
};

typedef enum {  // Token definition
  TK_RESERVED,  // operator symbols
  TK_IDENT,     // identifier
  TK_NUM,       // integer token
  TK_CHAR,      // single char
  TK_EOF,       // end of input
  TK_RETURN,    // return
  TK_SIZEOF,    // sizeof
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
Var *locals;
Var *globals;
int label_index;
extern char *builtin_type_names[];
extern int num_builtin_types;
extern TypeKind builtin_type_enum[];

//////////
// utility functions

void error(char *fmt, ...);
void error_at(char* loc, char *fmt, ...);

//////////
// token-related functions
Token *tokenize(char *p);

//////////
// ast-related functions

// find if the global var is already defined
Var *find_gvar_by_id(int offset);
Var *find_lvar_by_id(int offset);

// get a number of local variables
int get_num_lvars();

// Non-terminal symbols generator
void program();

// Code generator
void gen(Node *node);

//////////
// debug function
void print_node_tree();

//////////
// architecture-dependent functions

// type helper function
int size_var(Type* ty);
