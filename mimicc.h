#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef struct Const_Strings Const_Strings;

struct Const_Strings {
  int id;
  int size;
  Const_Strings *next;
  char *str;
};

typedef struct Type Type;

typedef enum {
  TYPE_CHAR,
  TYPE_INT,
  TYPE_ARITHMETIC_LIMIT,
  TYPE_VOID,
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
  int scope_id;
  int offset_bytes;
  Type *ty;
};

typedef struct Func Func;

struct Func {  // defined variables
  Func *next;
  char *name;
  int len;
  int num_args;
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
  ND_STRINGS,   // strings
  ND_EQUIV,     // ==
  ND_INEQUIV,  // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_LVAR,      // Local variable
  ND_GVAR,      // Global variable
  ND_LVARINIT,  // Local variable initialize
  ND_GVARINIT,  // Global variable initialize
  ND_ASSIGN,    // =
  ND_RETURN,    // return
  ND_BREAK,     // break
  ND_CONTINUE,  // continue
  ND_WHILE,     // while
  ND_FOR,       // for
  ND_IF,        // if
  ND_BLOCK,     // {} block
  ND_CALL,      // function call
  ND_FUNC,      // function
  ND_ADDR,      // address
  ND_DEREF,     // dereference
  ND_SIZEOF,    // sizeof
  ND_PREINC,   // pre ++
  ND_PREDEC,   // pre --
  ND_POSTINC,   // post ++
  ND_POSTDEC,   // post --
} NodeKind;

typedef struct Node Node;

struct Node { // ABS Node struct
  NodeKind kind;
  Node **children;
  int val;          // Only when kind==ND_NUM or ND_CALL or ND_GVAR
  int offset;       // Only when kind=ND_LVAR or ND_GVAR
  int id;           // Only when kind=ND_LVAR or ND_GVAR
  int num_args;     // Only when kind=ND_FUNC or ND_CALL
  char *func_name;  // Only when kind=ND_CALL
  Var *lvars;      // Only when kind=ND_FUNC
  Type *ty;
};

typedef enum {  // Token definition
  TK_RESERVED,  // operator symbols
  TK_IDENT,     // identifier
  TK_NUM,       // integer token
  TK_CHAR,      // single char
  TK_STRINGS,   // strings
  TK_EOF,       // end of input
} TokenKind;

typedef struct Token Token;

struct Token {  // Token type
  TokenKind kind;
  Token * next; // next input token
  int val;      // value if kind is TK_NUM
  char *str;    // token string
  int len;
};

typedef struct Scope Scope;

struct Scope {
  int id;
  Scope *parent;
};

//////////
// global variable definition
Token *token;         // current token pointer
Node **codes;
char *file_path;     // Input program path
char *user_input;     // Input program
Var *locals;
Var *globals;
Func *funcs;
Func *current_func;
Const_Strings *cstrs;
int label_index;
int last_scope_id;
int ctrl_depth;
Scope *current_scope;
extern char *builtin_type_names[];
extern int num_builtin_types;
extern TypeKind builtin_type_enum[];

//////////
// utility functions

void Error(char *fmt, ...);
void ErrorAt(char *loc, char *fmt, ...);
void WarnAt(char *loc, char *fmt, ...);

//////////
// token-related functions
Token *Tokenize(char *p);

//////////
// ast-related functions

// find if the global var is already defined
Var *FindGvarById(int offset);
Var *FindLvarById(int offset);

// get a number of local variables
int GetNumLvars();

// Non-terminal symbols generator
void program();

// Code generator
void Generate(Node *node);
void GenerateHeader();
void GenerateFooter();

//////////
// debug function
void PrintNodeTree();
void PrintLvar();

//////////
// architecture-dependent functions

// type helper function
int GetTypeSize(Type *ty);
