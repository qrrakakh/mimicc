#include "9cc.h"

//////////
// token-related functions

// Read one token and return true if the next token is an expected symbol,
// else return false
bool consume(char* op) {
  if(token->kind != TK_RESERVED ||
     strlen(op) != token-> len ||
     memcmp(token->str, op, token->len))
    return false;

  token = token->next;
  return true;
}

// Read one token if the next token is an expected symbol,
// else report an error
void expect(char* op) {
  if(token->kind != TK_RESERVED ||
     strlen(op) != token-> len ||
     memcmp(token->str, op, token->len))
    error_at(token->str, "The expected operator '%s' is not input.", op);
  token = token->next;
}

// Read one token and return the pointed token if the next token is a identifier,
// else report an error
Token *consume_ident() {
  if(token->kind != TK_IDENT)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

// Read one token and return the pointed token if the next token is a identifier,
// else report an error
Token *consume_return() {
  if(token->kind != TK_RETURN)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
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

// Gennm erate new token and concatenate to cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

int isidentchar(int p) {
  if (('a' <= p && p <= 'z') || ('A' <= p && p <= 'Z') || p == '_') {
    return 1;
  } else {
    return 0;
  }
}

int isreturn(char *p) {
  return strncmp(p, "return", 6)==0 && p+6 && isspace(*(p+6));
}

// Tokenize input string p
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;
  int l;

  while (*p) {
    // skip blank
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (isreturn(p)) {
      cur = new_token(TK_RETURN, cur, p, 6);
      p+=6;
      continue;
    }

    // local variable (starting from [a-z][A-Z]_ and following[a-z][A-Z][0-9]_)
    if (isidentchar(*p)) {
      for(l=1;(p+l)&&(isidentchar(*(p+l)) || isdigit(*(p+l)));++l);
      cur = new_token(TK_IDENT, cur, p, l);
      p+=l;
      continue;
    }

    // two chars operator
    if (strlen(p) >= 2) {
      if( (*p == '<' || *p == '>'  || *p == '='  || *p == '!' ) && *(p+1) == '=') {
        cur = new_token(TK_RESERVED, cur, p, 2);
        p+=2;
        continue;
      }
    }

    // one char operator
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>' || *p == '=' || *p == ';') {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 1); // token->str is only the first char of the digit?
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "Failed to tokenize");
  }

  new_token(TK_EOF, cur, p, 1);
  return head.next;
}

//////////
// ast-related functions

// Generate new node
Node *new_node_unaryop(NodeKind kind, Node *lhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = NULL;
  return node;
}

Node *new_node_binop(NodeKind kind, Node *lhs, Node *rhs) {
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

Node *new_node_lvar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  LVar *var = find_lvar(tok);
  
  if(!var) {
    var = calloc(1, sizeof(LVar));
    var->next = locals; locals = var;
    var->name = tok->str;
    var->len = tok->len;
    var->offset = var->next->offset + 8;
  }

  node->kind = ND_LVAR;
  node->offset = var->offset;
}

// find if the local var is already defined
LVar *find_lvar(Token *tok) {
  LVar *var;
  for(var=locals;var;var=var->next) {
    if(var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
      return var;
    }
  }
  return NULL;
}

// get a number of local variables
int get_num_lvars() {
  LVar *var = locals;
  int i = 0;
  while (var->next) {
    ++i;
    var = var->next;
  } 
  return i;
}

// Non-terminal symbols generator
void program() {
  int i=0;
  while(!at_eof()) {
    code[i++] = stmt();
  }
  code[i] = NULL;
}

Node *stmt() {
  Node *node;
  Token *tok = consume_return();
  if (tok) {
    node = new_node_unaryop(ND_RETURN, expr());
  } else {
    node = expr();
  }
  expect(";");
  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();
  for(;;) {
    if(consume("=")) {
      node = new_node_binop(ND_ASSIGN, node, assign());
    } else {
      return node;
    }
  }
}

Node *equality() {
  Node *node = relational();

  for(;;) {
    if (consume("=="))
      node = new_node_binop(ND_EQUIV, node, relational());
    else if (consume("!="))
      node = new_node_binop(ND_INEQUIV, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for(;;) {
    if (consume("<="))
      node = new_node_binop(ND_LE, node, add());
    else if (consume(">="))
      node = new_node_binop(ND_LE, add(), node);
    else if (consume("<"))
      node = new_node_binop(ND_LT, node, add());
    else if (consume(">"))
      node = new_node_binop(ND_LT, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for(;;) {
    if (consume("+"))
      node = new_node_binop(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node_binop(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for(;;) {
    if(consume("*"))
      node = new_node_binop(ND_MUL, node, unary());
    else if(consume("/"))
      node = new_node_binop(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if(consume("+"))
    return primary();
   else if(consume("-"))
    return new_node_binop(ND_SUB, new_node_num(0), primary());
  else
    return primary();
}

Node *primary() {
  Node *node;

  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (consume("(")) {
    node = expr();
    expect(")");
    return node;
  }

  Token *tok = consume_ident();
  if(tok) {
    return new_node_lvar(tok);
  } else { // else should be a number.
    return new_node_num(expect_number());
  }
}
