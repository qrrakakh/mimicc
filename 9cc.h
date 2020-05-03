#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef enum {  // ABS Node kinds
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
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
} NodeKind;

typedef struct Node Node;

struct Node { // ABS Node struct
  NodeKind kind;
  Node** children;
  int val;    // Only when kind==ND_NUM
  int offset; // Only when kind=ND_LVAR
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

typedef struct LVar LVar;

struct LVar {  // defined local variables
  LVar *next;
  char *name;
  int len;
  int offset;
};

//////////
// global variable definition
Token *token;         // current token pointer
Node *code[100];
char *user_input;     // Input program
LVar *locals;
int label_index;

//////////
// utility functions

void error(char *fmt, ...);
void error_at(char* loc, char *fmt, ...);

//////////
// token-related functions
void expect(char* op);
Token* consume(char* op);
Token* consume_ident();
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

//////////
// ast-related functions

// Generate new node
Node *new_node_unaryop(NodeKind kind, Node *valnode);
Node *new_node_binop(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_lvar(Token* tok);

// find if the local var is already defined
LVar *find_lvar(Token *tok);

// get a number of local variables
int get_num_lvars();


// Non-terminal symbols generator
void program();
Node* stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();



// Code generator
void gen(Node *node);




