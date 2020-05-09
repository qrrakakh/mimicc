#include "9cc.h"

//////////
// type-related functions
Type* type_int_init() {
  Type* ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_INT;
  ty->ptr_to = NULL;
  return ty;
}

//////////
// token-related functions

// Read one token and return the token if the next token is an expected symbol,
// else return NULL
Token *consume(char *op) {
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
void expect(char *op) {
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

Token *consume_typestr() {
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

// Read one token and return the pointed token if the next token is a identifier,
// else report an error
Token *consume_sizeof() {
  if(token->kind != TK_SIZEOF)
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

bool iskeyword(char *p, char *keyword, bool need_space) {
  int len = strlen(keyword);
  bool space_flg = (!need_space) | (p+len && isspace(*(p+len)));
  return strncmp(p, keyword, len)==0 && space_flg;
}

int istype(char* p) {
  int len = strlen("int");
  if(strncmp(p, "int", len)==0 && p+len && isspace(*(p+len)))
    return len;
  return 0;
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

    if (iskeyword(p, "sizeof", true) || iskeyword(p, "sizeof(", false)) {
      cur = new_token(TK_SIZEOF, cur, p, 6);
      p+=6;
      continue;
    }

    // primitive type
    if(l = istype(p)) {
      cur = new_token(TK_RESERVED, cur, p, l);
      p+=l;
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

LVar *find_lvar_by_offset(int offset) {
  LVar *var;
  for(var=locals;var;var=var->next) {
    if(var->offset == offset) {
      return var;
    }
  }
  return NULL;
}

// Generate new node
Node *new_node_unaryop(NodeKind kind, Node *valnode) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = valnode;

  switch(kind) {
    case ND_ADDR:
      node->ty = calloc(1, sizeof(Type));
      node->ty->kind = TYPE_PTR;
      node->ty->ptr_to = find_lvar_by_offset(node->children[0]->offset)->ty;
      break;
    case ND_DEREF:
      node->ty = find_lvar_by_offset(node->children[0]->offset)->ty->ptr_to;
      break;
    case ND_RETURN:
      node->ty = NULL; // TODO: check
      break;
    case ND_SIZEOF:
      node->ty = type_int_init();
      break;
  }
  return node;
}

Node *new_node_binop(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(2, sizeof(Node*));
  node->children[0] = lhs;
  node->children[1] = rhs;

  switch(kind) {
    case ND_WHILE:
      node->ty = NULL;
    break;
    // compare operator
    // support type: INT/INT
    case ND_EQUIV:
    case ND_INEQUIV:
    case ND_LE:
    case ND_LT:
      if (lhs->ty->kind != rhs->ty->kind) {
        error_at(token->str, 
          "different type cannot be compared; lhs type: %d, rhs type: %d",
          lhs->ty->kind, rhs->ty->kind);
      }
      node->ty = type_int_init();
      break;
    case ND_ASSIGN:
      if (lhs->ty->kind != rhs->ty->kind) {
        error_at(token->str, 
          "different type cannot be assigned; lhs type: %d, rhs type: %d, operation: %d",
          lhs->ty->kind, rhs->ty->kind, node->kind);
      }
      node->ty = rhs->ty;
      break;
    case ND_ADD:
      // add
      // support type: INT/INT, INT/PTR, PTR/INT
      if(lhs->ty->kind == TYPE_PTR && rhs->ty->kind == TYPE_PTR) {
        error_at(token->str, "adding pointer to pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    case ND_SUB:
      // sub
      // support type: INT/INT, PTR/INT (not INT/PTR)
      if(rhs->ty->kind == TYPE_PTR) {
        error_at(token->str, "subtracting pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    // mul/div
    // support type: INT/INT
    case ND_MUL:
    case ND_DIV:
    case ND_MOD:
      if (lhs->ty->kind != TYPE_INT || rhs->ty->kind != TYPE_INT) {
        error_at(token->str, 
        "non-int binary operation for mul/div/mod is not supported; lhs type: %d, rhs type: %d, operation: %d",
          lhs->ty->kind, rhs->ty->kind, node->kind);
      }
      node->ty = lhs->ty;
      break;
  }

  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_NUM;
  node->val = val;
  node->ty = type_int_init();
  return node;
}

Node *new_node_lvar(Token *tok, Type *ty, bool declare) {
  Node *node = calloc(1, sizeof(Node));
  LVar *var;
  
  if (declare) {
    var = calloc(1, sizeof(LVar));
    var->next = locals; locals = var;
    var->name = tok->str;
    var->len = tok->len;
    var->offset = var->next->offset + 1;
    var->ty = ty;
  } else {
    if(!(var=find_lvar(tok))) {
      error_at(token->str, "Undefined local variable.");
    }
  }

  node->children = NULL;
  node->kind = ND_LVAR;
  node->offset = var->offset;
  node->ty = var->ty;
  return node;
}

Node *new_node_for(Node *init, Node *cond, Node *next, Node *stmt) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FOR;
  node->children = calloc(4, sizeof(Node*));
  node->children[0] = init;
  node->children[1] = cond;
  node->children[2] = next;
  node->children[3] = stmt;
  node->ty = NULL;
  return node;
}

Node *new_node_if(Node *cond, Node *stmt1, Node *stmt2) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_IF;
  node->children = calloc(3, sizeof(Node*));
  node->children[0] = cond;
  node->children[1] = stmt1;
  node->children[2] = stmt2;
  node->ty = NULL;
  return node;
}

Node *new_node_block(Node **stmt_list) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_BLOCK;
  node->children = stmt_list;
  node->ty = NULL;
  return node;
}

Node *new_node_func(Token *tok, int num_arg, Node *block_node) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FUNC;
  node->children = calloc(1, sizeof(Node*));;
  node->children[0] = block_node;
  node->func_name = tok->str;
  node->val = tok->len;
  node->offset = num_arg;
  node->lvars = locals;
  node->ty = type_int_init(); // TODO: modify if non-int function is implemented.
  return node;
}

Node *new_node_funccall(Token *tok, int num_arg, Node *arg[]) {
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
  node->ty = type_int_init(); // TODO: modify if non-int function is implemented.
  return node;
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
// void program();
Node *func();
Node *block();
Node *stmt();
Type *type();
Node *declare();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();


void program() {
  int i=0;
  while(!at_eof()) {
    code[i++] = func();
  }
  code[i] = NULL;
}

Node *func() {
  Token *type_tok, *ident_tok, *arg_tok;
  Type *ty;
  Node *arg[6];
  int num_arg;
  if(!(ty = type())) {
    error_at(token->str, "Invalid type in function declaration.");
  }
  if(!(ident_tok=consume_ident())) {
    error_at(token->str, "Invalid function definition.");
  }

  // dummy lvar
  locals = calloc(1, sizeof(LVar)); 
  locals->next = NULL;
  locals->offset = 0;
  
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

    return new_node_block(stmt_list);
  } else {
    return NULL;
  }
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
    Node *init = expr();
    expect(";");
    Node *cond = expr();
    expect(";");
    Node *next = expr();
    expect(")");
    node = new_node_for(init, cond, next, stmt());
  } else if(tok = consume("if")) {
    expect("(");
    Node *cond = expr();
    expect(")");
    Node *stmt1 = stmt();
    Node *stmt2 = NULL;
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

Type *type() {
  Type* ty, *tgt_ty;
  Token* tok;
  if(!(tok = consume_typestr())) {
    return NULL;
  } 
  ty = type_int_init();
  while(consume("*")) {
    tgt_ty = ty;
    ty = calloc(1, sizeof(Type));
    ty->kind = TYPE_PTR;
    ty->ptr_to = tgt_ty;
  }
  return ty;
}

Node *declare() {
  Token *tok;
  Type *ty;
  if(!(ty = type()))
    return NULL;
  if(!(tok = consume_ident()))
    return NULL;
  return new_node_lvar(tok, ty, true);
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
  if (consume_sizeof()) {
    return new_node_unaryop(ND_SIZEOF, unary());
  }
  else if(consume("&"))
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
        arg[num_arg++] = expr();
        while(num_arg<=6) {
          if(consume(",")) {
            arg[num_arg++] = expr();
          } else {
            break;
          }
        }
        expect(")");
      }
      return new_node_funccall(tok, num_arg, arg);
    } else {
      return new_node_lvar(tok, NULL, false);
    }
  } else { // else should be a number.
    return new_node_num(expect_number());
  }
}
