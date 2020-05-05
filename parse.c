#include "9cc.h"

//////////
// token-related functions

// Read one token and return the token if the next token is an expected symbol,
// else return NULL
Token* consume(char* op) {
  Token *tok;
  if(token->kind != TK_RESERVED ||
     strlen(op) != token-> len ||
     memcmp(token->str, op, token->len))
    return NULL;

  tok = token;
  token = token->next;
  return tok;
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

Token *consume_type() {
  return consume("int");
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

bool iskeyword(char *p, char* keyword, bool need_space) {
  int len = strlen(keyword);
  bool space_flg = (!need_space) | isspace(*(p+len));
  return strncmp(p, keyword, len)==0 && p+len && space_flg;
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

    if (iskeyword(p, "return", true)) {
      cur = new_token(TK_RETURN, cur, p, 6);
      p+=6;
      continue;
    }

    // primitive type
    if(iskeyword(p, "int", true)) {
      cur = new_token(TK_RESERVED, cur, p, 3);
      p+=3;
      continue;
    }

    // control flow keywords
    if(iskeyword(p, "for", false)) {
      cur = new_token(TK_RESERVED, cur, p, 3);
      p+=3;
      continue;
    }
    if(iskeyword(p, "while", false)) {
      cur = new_token(TK_RESERVED, cur, p, 5);
      p+=5;
      continue;
    }
    if(iskeyword(p, "if", false)) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p+=2;
      continue;
    }
    if(iskeyword(p, "else", false)) {
      cur = new_token(TK_RESERVED, cur, p, 4);
      p+=4;
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

    // reserved once char
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%' || *p == '&'
        || *p == '(' || *p == ')'
        || *p == '<' || *p == '>' || *p == '='
        ||*p == ';' || *p == ',' || *p == '{' || *p == '}') {
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
Node *new_node_unaryop(NodeKind kind, Node *valnode) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = valnode;
  return node;
}

Node *new_node_binop(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(2, sizeof(Node*));
  node->children[0] = lhs;
  node->children[1] = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_lvar(Token *tok, bool declare) {
  Node *node = calloc(1, sizeof(Node));
  LVar *var;
  
  if (declare) {
    var = calloc(1, sizeof(LVar));
    var->next = locals; locals = var;
    var->name = tok->str;
    var->len = tok->len;
    var->offset = var->next->offset + 8;
  } else {
    if(!(var=find_lvar(tok))) {
      error_at(token->str, "Undefined local variable.");
    }
  }

  node->children = NULL;
  node->kind = ND_LVAR;
  node->offset = var->offset;
  return node;
}

Node *new_node_for(Node *init, Node *cond, Node *next, Node* stmt) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FOR;
  node->children = calloc(4, sizeof(Node*));
  node->children[0] = init;
  node->children[1] = cond;
  node->children[2] = next;
  node->children[3] = stmt;
  return node;
}

Node *new_node_if(Node *cond, Node *stmt1, Node* stmt2) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_IF;
  node->children = calloc(3, sizeof(Node*));
  node->children[0] = cond;
  node->children[1] = stmt1;
  node->children[2] = stmt2;
  return node;
}

Node *new_node_block(Node **stmt_list) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_BLOCK;
  node->children = stmt_list;
  return node;
}

Node *new_node_func(Token* tok, int num_arg, Node *block_node) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FUNC;
  node->children = calloc(1, sizeof(Node*));;
  node->children[0] = block_node;
  node->func_name = tok->str;
  node->val = tok->len;
  node->offset = num_arg;
  node->lvars = locals;
  return node;
}

Node* new_node_funccall(Token *tok, int num_arg, Node *arg[]) {
  int i;
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_CALL;
  node->children = calloc(num_arg, sizeof(Node));
  for(i=0;i<num_arg;++i) {
    node->children[i] = arg[i];
  }
  node->func_name = tok->str;
  node->val = tok->len;
  node->offset = num_arg;
  return node;
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
    code[i++] = func();
  }
  code[i] = NULL;
}

Node *func() {
  Token *type_tok, *ident_tok, *arg_tok;
  Node *arg[6];
  int num_arg;
  if(!(type_tok = consume_type())) {
    error_at(token->str, "Invalid type in function declaration.");
  }
  if(!(ident_tok=consume_ident())) {
    error_at(token->str, "Invalid function definition.");
  }

  // dummy lvar
  locals = calloc(1, sizeof(LVar)); 
  locals->next = NULL;

  expect("(");
  num_arg = 0;
  if(!consume(")")) {
    arg[num_arg++] = declare();
    while(num_arg<=6) {
      if(consume(",")) {
          arg[num_arg++] = declare();
      } else {
        break;
      }
    }
    expect(")");
  }
  
  return new_node_func(ident_tok, num_arg, block());
}

Node *block() {
  if(consume("{")) {

    Node **stmt_list;
    int alloc_unit = 10;
    int alloc_size = alloc_unit;
    int cur = 0;
    
    stmt_list = calloc(alloc_unit, sizeof(Node*));
    while(!(consume("}"))) {
      if(cur>=alloc_size-1) {
        alloc_size += alloc_unit;
        Node **_stmt_list = realloc(stmt_list, alloc_size);
        if(_stmt_list == NULL) {
          error("Memory allocation failure.");
        }
        stmt_list = _stmt_list;
      }
      stmt_list[cur++] = stmt();
    }
    stmt_list[cur] = NULL;

    // return new_node_func
    return new_node_block(stmt_list);
  } else {
    return NULL;
  }
}

Node* declare() {
  Token *tok;
  if(!consume_type())
    return NULL;
  if(!(tok = consume_ident()))
    return NULL;
  return new_node_lvar(tok, true);
}

Node *stmt() {
  Node *node;
  Token *tok;

  if (tok = consume_return()) {
    node = new_node_unaryop(ND_RETURN, expr());
    expect(";");
  } else if(node = declare()) {
    expect(";");
  } else if (node = block()) {
    return node;
  } else if(tok = consume("while")) {
    expect("(");
    Node *cond = expr();
    expect(")");
    node = new_node_binop(ND_WHILE, cond, stmt());
  } else if(tok = consume("for")) {
    expect("(");
    Node* init = expr();
    expect(";");
    Node* cond = expr();
    expect(";");
    Node* next = expr();
    expect(")");
    node = new_node_for(init, cond, next, stmt());
  } else if(tok = consume("if")) {
    expect("(");
    Node* cond = expr();
    expect(")");
    Node* stmt1 = stmt();
    Node* stmt2 = NULL;
    if (tok = consume("else")) {
      stmt2 = stmt();
    }
    node = new_node_if(cond, stmt1, stmt2);
  } else {
    node = expr();
    expect(";");
  }
  
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
    else if(consume("\%"))
      node = new_node_binop(ND_MOD, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if(consume("&"))
    return new_node_unaryop(ND_ADDR, unary());
  else if(consume("*"))
    return new_node_unaryop(ND_DEREF, unary());
  else if(consume("+"))
    return primary();
   else if(consume("-"))
    return new_node_binop(ND_SUB, new_node_num(0), primary());
  else
    return primary();
}

Node *primary() {
  Node *node;
  Node *arg[6];
  int num_arg;

  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (consume("(")) {
    node = expr();
    expect(")");
    return node;
  }

  Token *tok = consume_ident();
  if(tok) {
    if (consume("(")) {
      num_arg = 0;
      if(!consume(")")) {
        arg[num_arg++] = primary();
        while(num_arg<=6) {
          if(consume(",")) {
            arg[num_arg++] = primary();
          } else {
            break;
          }
        }
        expect(")");
      }
      return new_node_funccall(tok, num_arg, arg);
    } else {
      return new_node_lvar(tok, false);
    }
  } else { // else should be a number.
    return new_node_num(expect_number());
  }
}
