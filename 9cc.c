#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////
// struct definition

typedef enum {  // ABS Node kinds
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // integer
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
};

//////////
// global variable definition
Token *token;         // current token pointer
char *user_input;     // Input program

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
void error_at(char* loc, char *fmt, ...) {
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

//////////
// token-related functions

// Read one token and return true if the next token is an expected symbol,
// else return false
bool consume(char op) {
  if(token->kind != TK_RESERVED || token->str[0] != op)
    return false;

  token = token->next;
  return true;
}

// Read one token if the next token is an expected symbol,
// else report an error
void expect(char op) {
  if(token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "The expected operator '%c' is not input.", op);
  token = token->next;
}

// Read one token and return the current value if the next token is a value,
// else report an error
int expect_number() {
  if(token->kind != TK_NUM)
    error_at(token->str, "The number is expected.");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// Generate new token and concatenate to cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Tokenize input string p
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // skip blank
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p); // token->str is only the first char of the digit?
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "Failed to tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

//////////
// ast-related functions

// Generate new node
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
}

// Code generator
void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch(node->kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
  }

  printf("  push rax\n");
}

// Non-terminal symbols generator
Node *expr();
Node *mul();
Node *primary();

Node *expr() {
  Node *node = mul();

  for(;;) {
    if (consume('+'))
      node = new_node(ND_ADD, node, mul());
    else if (consume('-'))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = primary();

  for(;;) {
    if(consume('*'))
      node = new_node(ND_MUL, node, primary());
    else if(consume('/'))
      node = new_node(ND_DIV, node, primary());
    else
      return node;
  }
}

Node *primary() {
  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }

  // else should be a number.
  return new_node_num(expect_number());
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // tokenize and parse;
  user_input = argv[1];
  token = tokenize(user_input);
  Node *node = expr();

  // The header of assembler
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Follow AST and generate code
  gen(node);

  // Finally load the top value of the stack
  // and return this.
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}