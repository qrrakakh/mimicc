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
//  ND_GT,        // >
//  ND_GE,        // >=
} NodeKind;

typedef struct Node Node;

struct Node { // ABS Node struct
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;    // Only when kind==ND_NUM
};

typedef enum {  // Token definition
  TK_RESERVED,  // operator symbols
  TK_NUM,       // integer token
  TK_EOF,       // end of input
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
char *user_input;     // Input program

//////////
// utility functions

void error(char *fmt, ...);
void error_at(char* loc, char *fmt, ...);

//////////
// token-related functions
bool consume(char* op);
void expect(char* op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

//////////
// ast-related functions

// Generate new node
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

// Non-terminal symbols generator
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();



// Code generator
void gen(Node *node);




