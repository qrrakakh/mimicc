#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef struct Const_Strings Const_Strings;
typedef struct Node Node;
typedef struct Type Type;
typedef struct DeclSpec DeclSpec;
typedef struct Symbol Symbol;
typedef struct Func Func;
typedef struct Token Token;
typedef struct Scope Scope;
typedef struct Struct Struct;
typedef struct Enum Enum;
typedef enum {
  NOSTORAGESPEC,
  EXTERN,
  TYPEDEF,
  STATIC,
  AUTO,
  REGISTER,
} StorageSpec;
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
typedef enum {
  SY_VAR,       // variable
  SY_FUNC,      // function
  SY_ENUMCONST, // enum const
  SY_TYPEDEF,   // typedef
} SymbolKind;
typedef enum {  // Token definition
  TK_RESERVED,  // reserved keywords / punctuators
  TK_IDENT,     // identifier
  TK_NUM,       // number token
  TK_CHAR,      // single char
  TK_STRINGS,   // strings
  TK_EOF,       // end of input
} TokenKind;
typedef enum {
  TQ_CONST    = 1,      // const
  TQ_RESTRICT = 1<<1,   // restrict
  TQ_VOLATILE = 1<<2,   // volatile
} TypeQual;

struct Const_Strings {
  Token *tok;
  int id;
  Const_Strings *next;
};

struct Type {
  TypeKind kind;
  struct Type *ptr_to;
  size_t array_size;
  _Bool is_variable_length;
  int id;
  TypeQual tq;
};

struct DeclSpec {
  Type *ty;
  StorageSpec sspec;
};

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

struct Func {  // defined variables
  Func *next;
  Symbol *symbol;
  int num_args;
  _Bool is_defined;
  _Bool has_varargs;
};

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
  ND_LNOT,      // !
  ND_LAND,      // &&
  ND_LOR,       // ||
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
  ND_CONDEXPR,  // conditional statement
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

struct Scope {
  int id;
  Scope *parent;
};

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

struct Enum {
  Token *tok;
  int id;
  int scope_id;
  Node *constants;
  Type *ty;
  Enum *next;
  _Bool is_defined;
};

//////////
// global variable definition
extern Token *token;         // current token pointer
extern Node **codes;
extern char *file_path;     // Input program path
extern char *user_input;     // Input program
extern Symbol *locals;
extern Symbol *globals;
extern Func *funcs;
extern Func *current_func;
extern Struct *structs;
extern Enum *enums;
extern int last_symbol_id;
extern int last_struct_id;
extern int last_enum_id;
extern Node *current_switch;
extern Const_Strings *cstrs;
extern int label_index;
extern int last_scope_id;
extern int ctrl_depth;
extern Scope *current_scope;

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
// ascii function
char GetEscapedChar(char c);

//////////
// debug function
void PrintNodeTree();
void PrintLvar();

//////////
// architecture-dependent functions

// type helper function
int GetTypeSize(Type *ty);
int GetStructMemberOffset(int struct_id, int member_id);

//////////
// evaluator for constant
int Eval(Node *node);
