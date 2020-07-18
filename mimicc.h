#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef struct Const_Strings Const_Strings;
typedef struct Node Node;


struct Const_Strings {
  int id;
  int size;
  Const_Strings *next;
  char *str;
};

typedef enum {
  NOSTORAGESPEC,
  EXTERN,
  TYPEDEF,
  STATIC,
  AUTO,
  REGISTER,
} StorageSpec;

typedef struct Type Type;

typedef enum {
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_INT,
  TYPE_ENUM,
  TYPE_ARITHMETIC_LIMIT,
  TYPE_VOID,
  TYPE_PTR,
  TYPE_ARRAY,
  TYPE_STRUCT,
} TypeKind;

struct Type {
  TypeKind kind;
  struct Type *ptr_to;
  size_t array_size;
  int id;
};

typedef struct DeclSpec DeclSpec;

struct DeclSpec {
  Type *ty;
  StorageSpec sspec;
};

typedef enum {
  SY_VAR,       // variable
  SY_FUNC,      // function
  SY_ENUMCONST, // enum const
  SY_TYPEDEF,   // typedef
} SymbolKind;

typedef struct Symbol Symbol;

struct Symbol {  // defined variables
  Symbol *next;
  Symbol *prev;
  SymbolKind kind;
  char *name;
  int len;
  int id;
  int scope_id;
  int offset_bytes;
  int val;
  Type *ty;
  Node *initializer;
};

typedef struct Func Func;

struct Func {  // defined variables
  Func *next;
  Symbol *symbol;
  int num_args;
  _Bool is_defined;
};

typedef enum {  // Token definition
  TK_RESERVED,  // reserved keywords / punctuators
  TK_IDENT,     // identifier
  TK_NUM,       // number token
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

typedef enum {  // ABS Node kinds
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_MOD,       // %
  ND_NUM,       // integer
  ND_CHAR,      // char
  ND_STRINGS,   // strings
  ND_AND,       // &
  ND_OR,        // |
  ND_XOR,       // ^
  ND_NOT,       // ~
  ND_EQUIV,     // ==
  ND_INEQUIV,  // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_LSHIFT,    // <<
  ND_RSHIFT,    // >>
  ND_DUMMY,     // Dummy node, nothing to be compiled.
  ND_LIST,      // List node
  ND_IDENT,     // identifier
  ND_LVAR,      // Local variable
  ND_GVAR,      // Global variable
  ND_ASSIGN,    // =
  ND_RETURN,    // return
  ND_BREAK,     // break
  ND_CONTINUE,  // continue
  ND_WHILE,     // while
  ND_FOR,       // for
  ND_SWITCH,    // switch
  ND_CASE,      // case
  ND_SWLABEL,   // label for switch
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
  ND_ARROW,     // ->
  ND_DOT,       // .
  ND_INIT,      // initializer
} NodeKind;

struct Node { // ABS Node struct
  NodeKind kind;
  Node **children;
  int val;
  int offset;
  int id;
  int num_args;
  char *name;
  Symbol *lvars;
  Token *tok;
  Type *ty;
};

typedef struct Scope Scope;

struct Scope {
  int id;
  Scope *parent;
};

typedef struct Struct Struct;

struct Struct {
  Token *tok;
  int id;
  int size;
  int scope_id;
  Symbol *members;
  Type *ty;
  Struct *next;
  _Bool is_defined;
};

typedef struct Enum Enum;

struct Enum {
  char *name;
  int len;
  int id;
  int scope_id;
  Node *constants;
  Type *ty;
  Enum *next;
};

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

extern char *builtin_type_names[];
extern int num_builtin_types;
extern TypeKind builtin_type_enum[];
Type **builtin_type_obj;
Type *int_type;
Type *char_type;
Type *void_type;
Type *bool_type;

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
Symbol *FindGvarById(int offset);
Symbol *FindLvarById(int offset);
Func *FindFuncByName(char *name, int name_len);

// Struct
Struct *FindStructById(int struct_id);

// Strings
Const_Strings *FindCstrById(int struct_id);

// Non-terminal symbols generator
void translation_unit();

// Code generator
void Generate(Node *node);
void InitProgram();
void GenerateFooter();

//////////
// debug function
void PrintNodeTree();
void PrintLvar();

//////////
// architecture-dependent functions

// type helper function
int GetTypeSize(Type *ty);
int GetStructMemberOffset(int struct_id, int member_id);