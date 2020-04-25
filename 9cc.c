#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token definition
typedef enum {
  TK_RESERVED,  // operator symbols
  TK_NUM,       // integer token
  TK_EOF,       // end of input
} TokenKind;

typedef struct Token Token;

// Token type
struct Token {
  TokenKind kind;
  Token * next; // next input token
  int val;      // value if kind is TK_NUM
  char* str;    // token string
};

// current token pointer
Token *token;

// error reporter which takes same args to printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

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
    error("The expected operator '%c' is not input.", op);
  token = token->next;
}

// Read one token and return the current value if the next token is a value,
// else report an error
int expect_number() {
  if(token->kind != TK_NUM)
    error("The number is expected.");
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

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p); // token->str is only the first char of the digit?
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("Failed to tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // tokenize
  token = tokenize(argv[1]);

  // The header of assembler
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Check if the begining of the token is a number
  // and then output the first mov op
  printf("  mov rax, %ld\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %ld\n", expect_number());
      continue;
    }

    if (consume('-')) {
      printf("  sub rax, %ld\n", expect_number());
      continue;
    }

    error("Unexpected token");
  }

  printf("  ret\n");
  return 0;
}