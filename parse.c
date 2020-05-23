#include "9cc.h"

//////////
// type-related functions
Type *type_init(TypeKind t) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = t;
  ty->ptr_to = NULL;
}

Type *type_int_init() {
  return type_init(TYPE_INT);
}

Type *type_char_init() {
  return type_init(TYPE_CHAR);
}

Type *type_array_init(Type *_ty, size_t size) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_ARRAY;
  ty->ptr_to = _ty;
  ty->array_size = size;
  return ty;
}

int isarithmetictype(Type* ty) {
  return ty->kind < TYPE_ARITHMETIC_LIMIT;
}

//////////
// parse functions

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
  Token *tok;
  for(int i=0;i<num_builtin_types;++i) {
    if(tok=consume(builtin_type_names[i]))
      return tok;
  }
  return NULL;
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

Token *consume_char() {
  if(token->kind != TK_CHAR)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

Token *consume_strings() {
  if(token->kind != TK_STRINGS)
    return NULL;
  Token *tok = token;
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

////////
// ast-related functions

// find if the local var is already defined
Var *find_var(Token *tok, Var *head) {
  Var *var;
  for(var=head;var;var=var->next) {
    if(var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
      return var;
    }
  }
  return NULL;
}

Var *find_lvar(Token *tok) {
  return find_var(tok, locals);
}

Var *find_gvar(Token *tok) {
  return find_var(tok, globals);
}

Var *find_var_by_id(int id, Var *head) {
  Var *var;
  for(var=head;var;var=var->next) {
    if(var->id == id) {
      return var;
    }
  }
  return NULL;
}

Var *find_lvar_by_id(int id) {
  return find_var_by_id(id, locals);
}

Var *find_gvar_by_id(int id) {
  return find_var_by_id(id, globals);
}

Const_Strings *find_cstr(char* s, int l) {
  Const_Strings *cs = cstrs, *new_cs;
  while(cs->next) {
    if (strncmp(cs->str, s, cs->size)==0) {
      return cs;
    }
    cs = cs->next;
  }

  new_cs = calloc(1, sizeof(Const_Strings));
  new_cs->next = cstrs; cstrs = new_cs;
  new_cs->id = new_cs->next->id + 1;
  new_cs->size = l;
  new_cs->str = s;
  return new_cs;
}

bool isglobalvar(Token* tok) {
  if(find_lvar(tok))
    return false;
  else if(find_gvar(tok))
    return true;
  else
    error_at(token->str, "Undefined variable.");
}

// Generate new node
Node *new_node_unaryop(NodeKind kind, Node *valnode) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = valnode;

  switch(kind) {
    case ND_SIZEOF:
      node->ty = type_int_init();
      break;
    case ND_ADDR:
      node->ty = calloc(1, sizeof(Type));
      node->ty->kind = TYPE_PTR;
      node->ty->ptr_to = node->children[0]->ty;
      break;
    case ND_DEREF:
      node->ty = node->children[0]->ty->ptr_to;
      if(node->children[0]->ty->kind == TYPE_ARRAY) {
        node->children[0] = new_node_unaryop(ND_ADDR, node->children[0]);
      }
      break;
    case ND_RETURN:
      if(node->children[0]->ty->kind == TYPE_ARRAY) {
        node->children[0] = new_node_unaryop(ND_ADDR, node->children[0]);
      }
      node->ty = NULL; // TODO: check
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

  if(lhs->ty->kind == TYPE_ARRAY) {
    node->children[0] = new_node_unaryop(ND_ADDR, lhs);
    node->children[0]->ty->ptr_to = lhs->ty->ptr_to;  // type should be a pointer of the array target
    lhs = node->children[0];
  }

  if(rhs->ty->kind == TYPE_ARRAY) {
    node->children[1] = new_node_unaryop(ND_ADDR, rhs);
    node->children[1]->ty->ptr_to = rhs->ty->ptr_to;  // type should be a pointer of the array target
    rhs = node->children[1];
  }

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
      if ((!isarithmetictype(lhs->ty)) ||
          (!isarithmetictype(rhs->ty)) ||
          (lhs->ty->kind != rhs->ty->kind)) {
        error_at(token->str, 
          "both types should be arithmetic or different type cannot be compared; lhs type: %d, rhs type: %d",
          lhs->ty->kind, rhs->ty->kind);
      }
      node->ty = type_int_init();
      break;
    case ND_ASSIGN:
      if (isarithmetictype(lhs->ty) && isarithmetictype(rhs->ty)) {
        node->ty = lhs->ty;
      } else if (lhs->ty->kind != rhs->ty->kind) {
        error_at(token->str, 
          "different type cannot be assigned; lhs type: %d, rhs type: %d",
          lhs->ty->kind, rhs->ty->kind, node->kind);
      } else {
        node->ty = lhs->ty;
      }
      break;
    case ND_ADD:
      // add
      // support type: ARI/ARI, ARI/PTR, PTR/ARI
      if(lhs->ty->kind == TYPE_PTR && rhs->ty->kind == TYPE_PTR) {
        error_at(token->str, "adding pointer to pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    case ND_SUB:
      // sub
      // support type: ARI/ARI, PTR/ARI (not ARI/PTR)
      if(rhs->ty->kind == TYPE_PTR) {
        error_at(token->str, "subtracting pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    // mul/div
    // support type: ARI/ARI
    case ND_MUL:
    case ND_DIV:
    case ND_MOD:
      if ((!isarithmetictype(lhs->ty)) ||
          (!isarithmetictype(rhs->ty))) {
        error_at(token->str, 
        "non-int binary operation for mul/div/mod is not supported; lhs type: %d, rhs type: %d, operation: %d",
          lhs->ty->kind, rhs->ty->kind, node->kind);
      }
      node->ty = lhs->ty;
      break;
  }

  return node;
}

Node *new_node_char(char c) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_CHAR;
  node->val = c;
  node->ty = type_char_init();
  return node;
}

Node *new_node_strings(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_STRINGS;
  node->id = find_cstr(tok->str, tok->len)->id;
  node->ty = type_array_init(type_char_init(), tok->len);
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
  Var *var;
  
  if (declare) {
    var = calloc(1, sizeof(Var));
    var->next = locals; locals = var;
    var->name = tok->str;
    var->len = tok->len;
    var->ty = ty;
    if (ty->kind == TYPE_ARRAY) {
      var->id = var->next->id + ty->array_size;
    } else {
      var->id = var->next->id + 1;
    }
    
  } else {
    if(!(var=find_lvar(tok))) {
      error_at(token->str, "Undefined local variable.");
    }
  }

  node->children = NULL;
  node->kind = ND_LVAR;
  node->id = var->id;
  node->ty = var->ty;
  return node;
}

Node *new_node_gvar(Token *tok, Type *ty, bool declare) {
  Node *node = calloc(1, sizeof(Node));
  Var *var;
  
  if (declare) {
    var = calloc(1, sizeof(Var));
    var->next = globals; globals = var;
    var->name = tok->str;
    var->len = tok->len;
    var->ty = ty;
    if (ty->kind == TYPE_ARRAY) {
      var->id = var->next->id + ty->array_size;
    } else {
      var->id = var->next->id + 1;
    }
    node->val = 1;
    
  } else {
    if(!(var=find_gvar(tok))) {
      error_at(token->str, "Undefined global variable.");
    }
    node->val = 0;
  }

  node->children = NULL;
  node->kind = ND_GVAR;
  node->id = var->id;
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

Node *new_node_func(Token *tok, int num_args, Node *block_node) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FUNC;
  node->children = calloc(1, sizeof(Node*));;
  node->children[0] = block_node;
  node->func_name = tok->str;
  node->val = tok->len;
  node->num_args = num_args;
  node->lvars = locals;
  node->ty = type_int_init(); // TODO: modify if non-int function is implemented.
  return node;
}

Node *new_node_funccall(Token *tok, int num_args, Node *arg[]) {
  int i;
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_CALL;
  node->children = calloc(num_args, sizeof(Node));
  for(i=0;i<num_args;++i) {
    node->children[i] = arg[i];
  }
  node->func_name = tok->str;
  node->val = tok->len;
  node->num_args = num_args;
  node->ty = type_int_init(); // TODO: modify if non-int function is implemented.
  return node;
}

// get a number of local variables
int get_num_lvars() {
  Var *var = locals;
  int i = 0;
  while (var->next) {
    if(var->id>i) i=var->id;
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
Node *declare_a();
Node *declare_g();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
Node *strings();


void program() {
  int i=0;
  Token *_tok;
  Node *node;
  Type *ty;

  globals = calloc(1, sizeof(Var)); 
  globals->next = NULL;
  globals->id = 0;

  cstrs = calloc(1, sizeof(Const_Strings));
  cstrs->next = NULL;
  cstrs->id = 0;

  while(!at_eof()) {
    _tok = token;
    code[i] = NULL;

    ty = type();
    if(!consume_ident()) {
      error_at(token->str, "Invalid definition statement.");
    }
    if(consume("(")) { // func
      token = _tok;
      code[i++] = func();
    } else if(consume("[") || consume(";")) { // global variable
      token = _tok;
      code[i++] = declare_g();
      expect(";");
    } else {
      token = _tok;
      error_at(token->str, "Definition of the global variable or the function should be allowed.");
    }
  }
  code[i] = NULL;
}

Node *func() {
  Token *type_tok, *ident_tok, *arg_tok;
  Type *ty;
  Node *arg[6];
  int num_args;
  if(!(ty = type())) {
    error_at(token->str, "Invalid type in function declaration.");
  }
  if(!(ident_tok=consume_ident())) {
    error_at(token->str, "Invalid function definition.");
  }

  // dummy lvar
  locals = calloc(1, sizeof(Var)); 
  locals->next = NULL;
  locals->id = 0;
  
  expect("(");
  num_args = 0;
  if(!consume(")")) {
    arg[num_args++] = declare();
    while(num_args<=6) {
      if(consume(",")) {
          arg[num_args++] = declare();
      } else {
        break;
      }
    }
    expect(")");
  }
  
  return new_node_func(ident_tok, num_args, block());
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
  } else if(node = declare_a()) {
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
  for(int i=0;i<num_builtin_types;++i) {
    if(strncmp(tok->str, builtin_type_names[i], tok->len)==0) {
      ty = type_init(builtin_type_enum[i]);
    }
  }

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

Node *declare_a() {
  Token *tok;
  Type *ty;
  size_t size;
  if(!(ty = type()))
    return NULL;
  if(!(tok = consume_ident()))
    return NULL;
  if (consume("[")) {
    size = expect_number();
    if(size<1) {
      error_at(token->str, "Array whose length less than 1 is invalid.");
    }
    ty = type_array_init(ty, size);
    expect("]");
  }
  return new_node_lvar(tok, ty, true);
}

Node *declare_g() {
  Token *tok;
  Type *ty;
  size_t size;
  if(!(ty = type()))
    return NULL;
  if(!(tok = consume_ident()))
    return NULL;
  if (consume("[")) {
    size = expect_number();
    if(size<1) {
      error_at(token->str, "Array whose length less than 1 is invalid.");
    }
    ty = type_array_init(ty, size);
    expect("]");
  }
  return new_node_gvar(tok, ty, true);
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
  Node *subscript;
  Node *arg[6];
  size_t num_args;
  Token *tok;

  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (consume("(")) {
    node = expr();
    expect(")");
    return node;
  }

  if(tok= consume_ident()) {
    if (consume("(")) {
      num_args = 0;
      if(!consume(")")) {
        arg[num_args++] = expr();
        while(num_args<=6) {
          if(consume(",")) {
            arg[num_args++] = expr();
          } else {
            break;
          }
        }
        expect(")");
      }
      return new_node_funccall(tok, num_args, arg);
    } else if(consume("[")) {
      subscript = expr();
      expect("]");
      if(isglobalvar(tok)) {
        return new_node_unaryop(ND_DEREF,
          new_node_binop(ND_ADD, new_node_gvar(tok, NULL, false), subscript));
      } else {
        return new_node_unaryop(ND_DEREF,
          new_node_binop(ND_ADD, new_node_lvar(tok, NULL, false), subscript));
      }
    } else {
      if(isglobalvar(tok)) {
        return new_node_gvar(tok, NULL, false);
      } else {
        return new_node_lvar(tok, NULL, false);
      }
    }
  } else if (tok=consume_char()) {
    return new_node_char(*(tok->str));
  } else if(node=strings()) {
    return node;
  } else { // else should be a number.
    return new_node_num(expect_number());
  }
}

Node *strings() {
  Token *tok;
  if (tok = consume_strings()) {
    return new_node_strings(tok);
  } else {
    return NULL;
  }
}