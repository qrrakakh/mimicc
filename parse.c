#include "mimicc.h"

//////////
// type-related functions
Type *InitType(TypeKind t) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = t;
  ty->ptr_to = NULL;
}

Type *InitIntType() {
  return InitType(TYPE_INT);
}

Type *InitCharType() {
  return InitType(TYPE_CHAR);
}

Type *InitArrayType(Type *_ty, size_t size) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_ARRAY;
  ty->ptr_to = _ty;
  ty->array_size = size;
  return ty;
}

int IsArithmeticType(Type *ty) {
  return ty->kind < TYPE_ARITHMETIC_LIMIT;
}

//////////
// scope related functions

void InitScope() {
  current_scope = calloc(1, sizeof(Scope));
  current_scope->id = 0;  // scope id for global namespace = 0
  current_scope->parent = NULL;
}

void EnterScope() {
  Scope *blk = calloc(1, sizeof(Scope));
  blk->id = ++last_scope_id;
  blk->parent = current_scope;
  current_scope = blk;
}

void LeaveScope() {
  current_scope = current_scope->parent;
}

//////////
// parse functions

Token *Consume(char *op) {
  Token *tok;
  if(token->kind != TK_RESERVED ||
     strlen(op) != token-> len ||
     memcmp(token->str, op, token->len))
    return NULL;

  tok = token;
  token = token->next;
  return tok;
}

Token *ConsumeIdent() {
  if(token->kind != TK_IDENT)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

Token *ConsumeTypeStr() {
  Token *tok;
  if(token->kind != TK_RESERVED)
    return NULL;
  for(int i=0;i<num_builtin_types;++i) {
    if(tok=Consume(builtin_type_names[i]))
      return tok;
  }
  return NULL;
}

Token *ConsumeChar() {
  if(token->kind != TK_CHAR)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

Token *ConsumeStrings() {
  if(token->kind != TK_STRINGS)
    return NULL;
  Token *tok = token;
  token = token->next;
  return tok;
}

void Expect(char *op) {
  if(token->kind != TK_RESERVED ||
     strlen(op) != token-> len ||
     memcmp(token->str, op, token->len))
    ErrorAt(token->str, "The expected operator '%s' is not input.", op);
  token = token->next;
}

int ExpectNumber() {
  if(token->kind != TK_NUM)
    ErrorAt(token->str, "The number is expected.");
  int val = token->val;
  token = token->next;
  return val;
}

bool AtEOF() {
  return token->kind == TK_EOF;
}

////////
// ast-related functions

// find if the local var is already defined
bool IsParentOfScopeId(int id, Scope *blk) {
  if (blk->id==id) {
    return true;
  } else if(blk->parent==NULL) {
    return false;
  } else {
    return IsParentOfScopeId(id, blk->parent);
  }
}

Func *FindFuncByName(char *name, int name_len) {
  Func *f;
  for(f=funcs;f;f=f->next) {
    if(f->len == name_len && !memcmp(f->name, name, f->len)) {
      return f;
    }
  }
  return NULL;
}

Func *FindFunc(Token *tok) {
  return FindFuncByName(tok->str, tok->len);
}

Func *AddFunc(Token *tok, Type *ty, int num_args) {
  Func *f;
  if(FindFunc(tok)) {
    ErrorAt(tok->str, "Function with existing name is declared again.");
  }
  f = calloc(1, sizeof(Func));
  f->next = funcs; funcs = f;
  f->name = tok->str;
  f->len = tok->len;
  f->num_args = num_args;
  f->ty = ty;
  return f;
}

Var *FindLvar(Token *tok, bool is_recursive_search) {
  Var *var;
  for(var=locals;var;var=var->next) {
    if(var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
      if(is_recursive_search) {
        if(IsParentOfScopeId(var->scope_id, current_scope))
          return var;
      } else {
        if(current_scope->id == var->scope_id)
          return var;
      }
    }
  }
  return NULL;
}

Var *AddLvar(Token *tok, Type *ty) {
  Var *var;
  if((var=FindLvar(tok, false))) {
    ErrorAt(tok->str, "Local variable with existing name is declared again.");
  } else if(ty->kind == TYPE_VOID) {
    ErrorAt(tok->str, "Local variable declared void.");
  }
  var = calloc(1, sizeof(Var));
  var->next = locals; var->prev = NULL;
  locals->prev = var; locals = var; 
  var->name = tok->str;
  var->len = tok->len;
  var->ty = ty;
  var->scope_id = current_scope->id;
  if (ty->kind == TYPE_ARRAY) {
    var->id = var->next->id + ty->array_size;
  } else {
    var->id = var->next->id + 1;
  }

  return var;
}

Var *FindGvar(Token *tok) {
  Var *var;
  for(var=globals;var;var=var->next) {
    if(var->len == tok->len && !memcmp(var->name, tok->str, var->len)) {
      return var;
    }
  }
  return NULL;
}

Var *FindVarById(int id, Var *head) {
  Var *var;
  for(var=head;var;var=var->next) {
    if(var->id == id) {
      return var;
    }
  }
  return NULL;
}

Var *AddGVar(Token *tok, Type *ty, bool is_extern) {
  Var *var;
  if((var=FindGvar(tok))) {
    ErrorAt(tok->str, "Global variable with existing name is declared again.");
  } else if(ty->kind == TYPE_VOID) {
    ErrorAt(tok->str, "Global variable declared void.");
  }
  var = calloc(1, sizeof(Var));
  var->next = globals; var->prev = NULL;
  globals->prev = var; globals = var;
  var->name = tok->str;
  var->len = tok->len;
  var->ty = ty;
  if(is_extern) {
    var->scope_id = -1;
  } else {
    var->scope_id = 0;
  }

  if (ty->kind == TYPE_ARRAY) {
    var->id = var->next->id + ty->array_size;
  } else {
    var->id = var->next->id + 1;
  }

  return var;
}

Var *FindLvarById(int id) {
  return FindVarById(id, locals);
}

Var *FindGvarById(int id) {
  return FindVarById(id, globals);
}

Const_Strings *FindCstr(char *s, int l) {
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

bool IsGlobalVar(Token *tok) {
  if(FindLvar(tok, true))
    return false;
  else if(FindGvar(tok))
    return true;
  else
    ErrorAt(tok->str, "Undefined variable.");
}

// Generate new node
Node *NewNodeUnaryOp(NodeKind kind, Node *valnode) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = valnode;

  switch(kind) {
    case ND_PREINC:
    case ND_PREDEC:
    case ND_POSTINC:
    case ND_POSTDEC:
      node->ty = valnode->ty;
    break;
    case ND_SIZEOF:
      node->ty = InitIntType();
      break;
    case ND_ADDR:
      node->ty = calloc(1, sizeof(Type));
      node->ty->kind = TYPE_PTR;
      node->ty->ptr_to = node->children[0]->ty;
      break;
    case ND_DEREF:
      node->ty = node->children[0]->ty->ptr_to;
      if(node->children[0]->ty->kind == TYPE_ARRAY) {
        node->children[0] = NewNodeUnaryOp(ND_ADDR, node->children[0]);
      }
      break;
    case ND_RETURN:
      if(node->children[0] && node->children[0]->ty->kind == TYPE_ARRAY) {
        node->children[0] = NewNodeUnaryOp(ND_ADDR, node->children[0]);
      }
      node->ty = NULL;
      break;
  }
  return node;
}

Node *NewNodeBinOp(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(2, sizeof(Node*));
  node->children[0] = lhs;
  node->children[1] = rhs;

  if(lhs->ty->kind == TYPE_ARRAY) {
    node->children[0] = NewNodeUnaryOp(ND_ADDR, lhs);
    node->children[0]->ty->ptr_to = lhs->ty->ptr_to;  // type should be a pointer of the array target
    lhs = node->children[0];
  }

  if(rhs->ty->kind == TYPE_ARRAY) {
    node->children[1] = NewNodeUnaryOp(ND_ADDR, rhs);
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
      if ((!IsArithmeticType(lhs->ty)) ||
          (!IsArithmeticType(rhs->ty)) ||
          (lhs->ty->kind != rhs->ty->kind)) {
        ErrorAt(token->str, 
          "both types should be arithmetic or different type cannot be compared; lhs type: %d, rhs type: %d, current scope id: %d",
          lhs->ty->kind, rhs->ty->kind, current_scope->id);
      }
      node->ty = InitIntType();
      break;
    case ND_ASSIGN:
      if (IsArithmeticType(lhs->ty) && IsArithmeticType(rhs->ty)) {
        node->ty = lhs->ty;
      } else if (lhs->ty->kind != rhs->ty->kind) {
        ErrorAt(token->str, 
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
        ErrorAt(token->str, "adding pointer to pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    case ND_SUB:
      // sub
      // support type: ARI/ARI, PTR/ARI (not ARI/PTR)
      if(rhs->ty->kind == TYPE_PTR) {
        ErrorAt(token->str, "subtracting pointer is not valid.");
      }
      node->ty = lhs->ty;
      break;
    // mul/div
    // support type: ARI/ARI
    case ND_MUL:
    case ND_DIV:
    case ND_MOD:
      if ((!IsArithmeticType(lhs->ty)) ||
          (!IsArithmeticType(rhs->ty))) {
        ErrorAt(token->str, 
        "non-int binary operation for mul/div/mod is not supported; lhs type: %d, rhs type: %d, operation: %d",
          lhs->ty->kind, rhs->ty->kind, node->kind);
      }
      node->ty = lhs->ty;
      break;
  }

  return node;
}

Node *NewNodeChar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_CHAR;
  node->val = *(tok->str);
  node->tok = tok;
  node->ty = InitCharType();
  return node;
}

Node *NewNodeStrings(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_STRINGS;
  node->id = FindCstr(tok->str, tok->len)->id;
  node->tok = tok;
  node->ty = InitArrayType(InitCharType(), tok->len);
  return node;
}

Node *NewNodeNum(Token *tok, int val) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_NUM;
  node->val = val;
  node->ty = InitIntType();
  return node;
}

Node *NewNodeIdent(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_IDENT;
  node->tok = tok;
  node->ty = NULL;
  return node;
}

Node *NewNodeVarInitializer(Node *var_node) {
  Node *node = calloc(1, sizeof(Node));
  if(current_scope->id==0)
    node->kind = ND_GVARINIT;
  else
    node->kind = ND_LVARINIT;
  node->children = calloc(2, sizeof(Node*));
  node->children[0] = var_node;
  node->children[1] =  NULL;
  node->ty = NULL;
  return node;
}

void AddVarInitializer(Node *init_node, Node *var_node) {
  while(init_node->children[1]) {
    init_node = init_node->children[1];
  }
  if(current_scope->id==0) {
    if(init_node->kind != ND_GVARINIT) {
      Error("Invalid node is passed for GVar init.");
    }

    if(var_node!=NULL) {
      init_node->children[1] = NewNodeVarInitializer(var_node);
    }
  } else {
    if(init_node->kind != ND_LVARINIT) {
      Error("Invalid node is passed for LVar init.");
    }

    if(var_node!=NULL) {
      init_node->children[1] = NewNodeVarInitializer(var_node);
    }

  }
}

Node *NewNodeLvar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  Var *var;

  if(!(var=FindLvar(tok, true))) {
    ErrorAt(tok->str, "Undefined local variable.");
  }

  node->children = NULL;
  node->kind = ND_LVAR;
  node->id = var->id;
  node->tok = tok;
  node->ty = var->ty;
  return node;
}

Node *NewNodeGvar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  Var *var;
  
  if(!(var=FindGvar(tok))) {
    ErrorAt(tok->str, "Undefined global variable.");
  }
  node->children=NULL;

  node->kind = ND_GVAR;
  node->id = var->id;
  node->tok = tok;
  node->ty = var->ty;
  return node;
}

Node *NewNodeFor(Node *init, Node *cond, Node *next, Node *stmt) {
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

Node *NewNodeSwitch(Node *cond) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_SWITCH;
  node->children = calloc(3, sizeof(Node*));
  node->children[0] = cond;
  node->children[1] = NULL;  // case lists
  node->children[2] = NULL;  // stmt
  node->ty = NULL;
  node->val = 0;  // label counter
  node->num_args = 1;
  return node;
}

Node *NewNodeSwLabel(int c) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_SWLABEL;
  node->children = NULL;
  node->val = c;
  return node;
}

Node *NewNodeSwCase(Node *expr, Node *next_node, int label_id) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_CASE;
  node->children = calloc(2, sizeof(Node));
  node->children[0] = expr;
  node->children[1] = next_node;
  node->val = label_id;
  return node;
}

Node *NewNodeIf(Node *cond, Node *stmt1, Node *stmt2) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_IF;
  node->children = calloc(3, sizeof(Node*));
  node->children[0] = cond;
  node->children[1] = stmt1;
  node->children[2] = stmt2;
  node->ty = NULL;
  return node;
}

Node *NewNodeBlock(Node **stmt_list) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_BLOCK;
  node->children = stmt_list;
  node->ty = NULL;
  return node;
}

Node *NewNodeControl(NodeKind k) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = k;
  node->children = NULL;
  node->ty = NULL;
  return node;
}

Node *NewNodeFunc(Token *tok, Type *ty, int num_args, Node *block_node) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FUNC;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = block_node;
  node->name = tok->str;
  node->val = tok->len;
  node->num_args = num_args;
  node->lvars = locals;
  node->ty = ty;
  return node;
}

Node *NewNodeFuncCall(Token *tok, int num_args, Node *arg[]) {
  int i;
  Func *f = FindFunc(tok);
  if(!f) {
    WarnAt(tok->str, "Implicitly declared function.");
    f = AddFunc(tok, InitIntType(), 0);
  }
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_CALL;
  node->children = calloc(num_args, sizeof(Node));
  for(i=0;i<num_args;++i) {
    node->children[i] = arg[i];
  }
  node->name = tok->str;
  node->val = tok->len;
  node->tok = tok;
  node->num_args = num_args;
  node->ty = f->ty;
  return node;
}

// Non-terminal symbols generator
void program();
Node *func(bool is_extern);
Node *block();
Node *stmt();
Type *type();
Node *declare();
Node *declare_a();
void declare_e();
void evar(Type *_ty);
Node *var_a(Type *_ty);
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
Node *postfix();
Node *const_();
Node *string();


void program() {
  int i=0;
  int alloc_unit = 10;
  int alloc_size = alloc_unit;

  Token *_tok;
  Node *node;
  Type *ty, *tgt_ty;

  globals = calloc(1, sizeof(Var)); 
  globals->next = NULL;
  globals->id = 0;

  cstrs = calloc(1, sizeof(Const_Strings));
  cstrs->next = NULL;
  cstrs->id = 0;

  funcs = calloc(1, sizeof(Func));
  funcs->next = NULL;

  last_scope_id = 0;
  ctrl_depth = 0;

  current_func = NULL;
  current_switch = NULL;
  InitScope();

  codes = calloc(alloc_size, sizeof(Node*));

  while(!AtEOF()) {

    if(i>=alloc_size-1) {
      alloc_size += alloc_unit;
      Node **_codes = realloc(codes, sizeof(Node*)*alloc_size);
      if(_codes == NULL) {
        Error("Memory allocation failure.");
      }
      codes = _codes;
    }

    _tok = token;
    
    if(Consume("extern")) {
      _tok = token;

      ty = type();

      while(Consume("*"));

      if(!ConsumeIdent()) {
        ErrorAt(token->str, "Invalid external declaration statement.");
      }

      if(Consume("(")) {
        token = _tok;
        node = func(true);
        Expect(";");
      } else if(Consume("[") ||Consume(",") || Consume(";")) { // global variable
        token = _tok;
        declare_e();
        Expect(";");
      } else {
        token = _tok;
        ErrorAt(token->str, "Definition of the global variable or the function should be allowed.");
      }
      continue;
    }

    ty = type();
    while(Consume("*"));
    if(!ConsumeIdent()) {
      ErrorAt(token->str, "Invalid definition statement.");
    }
    if(Consume("(")) { // func
      token = _tok;
      codes[i++] = func(false);
    } else if(Consume("[") ||Consume(",") || Consume(";")) { // global variable
      token = _tok;
      codes[i++] = declare_a(true);
      Expect(";");
    } else {
      token = _tok;
      ErrorAt(token->str, "Definition of the global variable or the function should be allowed.");
    }
  }
  codes[i] = NULL;
}

Node *func(bool is_extern) {
  Token *type_tok, *ident_tok, *arg_tok;
  Type *ty, *tgt_ty;
  Node *arg[6], *block_node;
  int num_args;
  if(!(ty = type())) {
    ErrorAt(token->str, "Invalid type in function declaration.");
  }

  while(Consume("*")) {
    tgt_ty = ty;
    ty = calloc(1, sizeof(Type));
    ty->kind = TYPE_PTR;
    ty->ptr_to = tgt_ty;
  }

  if(!(ident_tok=ConsumeIdent())) {
    ErrorAt(token->str, "Invalid function definition.");
  }

  // dummy lvar
  locals = calloc(1, sizeof(Var)); 
  locals->next = NULL;
  locals->id = 0;

  EnterScope();
  Expect("(");
  num_args = 0;
  if(!Consume(")")) {
    arg[num_args++] = declare();
    while(num_args<=6) {
      if(Consume(",")) {
        arg[num_args++] = declare();
      } else {
        break;
      }
    }
    Expect(")");
  }

  current_func=AddFunc(ident_tok, ty, num_args);

  if(is_extern) {
    LeaveScope();
    return NewNodeFunc(ident_tok, ty, num_args, NULL);
  } else{
    block_node = block();
    LeaveScope();
    return NewNodeFunc(ident_tok, ty, num_args, block_node);
  }
}

Node *block() {
  if(Consume("{")) {
    Node **stmt_list;
    int alloc_unit = 10;
    int alloc_size = alloc_unit;
    int cur = 0;

    EnterScope();
    
    stmt_list = calloc(alloc_size, sizeof(Node*));
    while(!(Consume("}"))) {
      if(cur>=alloc_size-1) {
        alloc_size += alloc_unit;
        Node **_stmt_list = realloc(stmt_list, sizeof(Node*)*alloc_size);
        if(_stmt_list == NULL) {
          Error("Memory allocation failure.");
        }
        stmt_list = _stmt_list;
      }
      stmt_list[cur++] = stmt();
    }
    
    LeaveScope();

    stmt_list[cur] = NULL;

    return NewNodeBlock(stmt_list);
  } else {
    return NULL;
  }
}

Node *stmt() {
  Node *node;
  Token *tok;

  if (tok = Consume("return")) {
    if(!current_func)
      ErrorAt(tok->str, "Returned outside of function.");
    if (current_func->ty->kind == TYPE_VOID) {
      if(Consume(";")) {
        node = NewNodeUnaryOp(ND_RETURN, NULL);
      } else {
        WarnAt(tok->str, "Returned with a value in void function.");
        node = NewNodeUnaryOp(ND_RETURN, expr());
        Expect(";");
      }
    } else {
      if(tok = Consume(";")) {
        ErrorAt(tok->str, "Returned without a value in non-void function.");
      } else {
        node = NewNodeUnaryOp(ND_RETURN, expr());
        Expect(";");
      }
    }
    
  } else if(tok=Consume("break")) {
    Expect(";");
    if(ctrl_depth < 1)
      ErrorAt(tok->str, "break is used in non-control syntax.");
    node = NewNodeControl(ND_BREAK);
  } else if(tok=Consume("continue")) {
    Expect(";");
    if(ctrl_depth < 1)
      ErrorAt(tok->str, "continue is used in non-control syntax.");
    node = NewNodeControl(ND_CONTINUE);
  } else if(node = declare_a(false)) {
    Expect(";");
  } else if (node = block()) {
    return node;
  } else if(tok = Consume("while")) {
    Expect("(");
    Node *cond = expr();
    Expect(")");
    ++ctrl_depth;
    node = NewNodeBinOp(ND_WHILE, cond, stmt());
    --ctrl_depth;
  } else if(tok = Consume("for")) {
    EnterScope();
    Expect("(");
    Node *init;
    if(!(init=declare_a(false))) {
      init = expr();
    }
    Expect(";");
    Node *cond = expr();
    Expect(";");
    Node *next = expr();
    Expect(")");
    ++ctrl_depth;
    node = NewNodeFor(init, cond, next, stmt());
    --ctrl_depth;
    LeaveScope();
  } else if(tok = Consume("if")) {
    Expect("(");
    Node *cond = expr();
    Expect(")");
    Node *stmt1 = stmt();
    Node *stmt2 = NULL;
    if (tok = Consume("else")) {
      stmt2 = stmt();
    }
    node = NewNodeIf(cond, stmt1, stmt2);
  } else if(tok = Consume("switch")) {
    Expect("(");
    Node *cond = expr();
    Expect(")");
    Node *sw = current_switch;
    node = NewNodeSwitch(cond);
    current_switch = node;
    ++ctrl_depth;
    Node *statement = stmt();
    --ctrl_depth;
    node->children[2] = statement;
    current_switch = sw;
  } else if(tok = Consume("case")) {
    if (current_switch==NULL)
      ErrorAt(tok->str, "Invalid case use in non-switch statement.");
    tok = token;
    Node *cond = expr();
    Expect(":");
    if (cond->kind != ND_NUM) {
      ErrorAt(tok->str, "Non-number is invalid for case expression right now.");
    }
    int label_id = ++(current_switch->val);
    node = NewNodeSwLabel(label_id);
    Node *case_node = NewNodeSwCase(cond, current_switch->children[1], label_id);
    current_switch->children[1] = case_node;
  } else if(tok = Consume("default")) {
    Expect(":");
    node = NewNodeSwLabel(0);
    current_switch->num_args = 0;
  }   else {
    node = expr();
    Expect(";");
  }
  
  return node;
}

Type *type() {
  Type *ty;
  Token *tok;
  if(!(tok = ConsumeTypeStr())) {
    return NULL;
  }
  for(int i=0;i<num_builtin_types;++i) {
    if(strncmp(tok->str, builtin_type_names[i], tok->len)==0) {
      ty = InitType(builtin_type_enum[i]);
    }
  }

  return ty;
}

Node *declare() {
  Token *tok;
  Type *ty, *tgt_ty;
  if(!(ty = type()))
    return NULL;

  while(Consume("*")) {
    tgt_ty = ty;
    ty = calloc(1, sizeof(Type));
    ty->kind = TYPE_PTR;
    ty->ptr_to = tgt_ty;
  }

  if(!(tok = ConsumeIdent()))
    return NULL;

  AddLvar(tok, ty);

  return NULL;
}

Node *var_a(Type *_ty) {
  Token *tok, *ident_tok;
  Type *ty;
  Node *assign_node;
  size_t size;
  ty = _ty;

  if(!(ident_tok = ConsumeIdent()))
    return NULL;
  if (Consume("[")) {
    tok = token;
    size = ExpectNumber();
    if(size<1) {
      ErrorAt(tok->str, "Array whose length less than 1 is invalid.");
    }
    ty = InitArrayType(ty, size);
    Expect("]");
  }

  if (current_scope->id==0) {
    AddGVar(ident_tok, ty, false);
  } else {
    AddLvar(ident_tok, ty);
  }

  if (Consume("=")) {
    assign_node = NewNodeBinOp(ND_ASSIGN, NewNodeLvar(ident_tok), assign());
  } else {
    assign_node = NULL;
  }

  return assign_node;
}

void declare_e() {
  Token *tok;
  Type *ty, *tgt_ty, *orig_ty;
  Node *node;

  if(!(orig_ty = type()))
    return;

  ty = orig_ty;
  while(Consume("*")) {
    tgt_ty = ty;
    ty = calloc(1, sizeof(Type));
    ty->kind = TYPE_PTR;
    ty->ptr_to = tgt_ty;
  }

  evar(ty);

  while(Consume(",")) {
    ty = orig_ty;
    while(Consume("*")) {
      tgt_ty = ty;
      ty = calloc(1, sizeof(Type));
      ty->kind = TYPE_PTR;
      ty->ptr_to = tgt_ty;
    }
    evar(ty);
  }
}

void evar(Type *_ty) {
  Token *tok;
  Type *ty;
  Node *node, *init_node;
  size_t size;
  ty = _ty;

  if(!(tok = ConsumeIdent()))
    return;
  if (Consume("[")) {
    size = ExpectNumber();
    if(size<1) {
      ErrorAt(token->str, "Array whose length less than 1 is invalid.");
    }
    ty = InitArrayType(ty, size);
    Expect("]");
  }

  AddGVar(tok, ty, true);
}

Node *declare_a() {
  Token *tok;
  Type *ty, *tgt_ty, *orig_ty;
  Node *node;

  if(!(orig_ty = type()))
    return NULL;

  ty = orig_ty;
  while(Consume("*")) {
    tgt_ty = ty;
    ty = calloc(1, sizeof(Type));
    ty->kind = TYPE_PTR;
    ty->ptr_to = tgt_ty;
  }

  node = NewNodeVarInitializer(var_a(ty));

  while(Consume(",")) {
    ty = orig_ty;
    while(Consume("*")) {
      tgt_ty = ty;
      ty = calloc(1, sizeof(Type));
      ty->kind = TYPE_PTR;
      ty->ptr_to = tgt_ty;
    }
    AddVarInitializer(node, var_a(ty));
  }

  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();
  for(;;) {
    if(Consume("=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, assign());
    } else if(Consume("+=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_ADD, node, assign()));
    } else if(Consume("-=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_SUB, node, assign()));
    } else if(Consume("*=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_MUL, node, assign()));
    } else if(Consume("/=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_DIV, node, assign()));
    } else if(Consume("\%=")) {
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_MOD, node, assign()));
    } else {
      return node;
    }
  }
}

Node *equality() {
  Node *node = relational();

  for(;;) {
    if (Consume("=="))
      node = NewNodeBinOp(ND_EQUIV, node, relational());
    else if (Consume("!="))
      node = NewNodeBinOp(ND_INEQUIV, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for(;;) {
    if (Consume("<="))
      node = NewNodeBinOp(ND_LE, node, add());
    else if (Consume(">="))
      node = NewNodeBinOp(ND_LE, add(), node);
    else if (Consume("<"))
      node = NewNodeBinOp(ND_LT, node, add());
    else if (Consume(">"))
      node = NewNodeBinOp(ND_LT, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for(;;) {
    if (Consume("+"))
      node = NewNodeBinOp(ND_ADD, node, mul());
    else if (Consume("-"))
      node = NewNodeBinOp(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for(;;) {
    if(Consume("*"))
      node = NewNodeBinOp(ND_MUL, node, unary());
    else if(Consume("/"))
      node = NewNodeBinOp(ND_DIV, node, unary());
    else if(Consume("\%"))
      node = NewNodeBinOp(ND_MOD, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (Consume("sizeof"))
    return NewNodeUnaryOp(ND_SIZEOF, unary());
  else if(Consume("&"))
    return NewNodeUnaryOp(ND_ADDR, unary());
  else if(Consume("*"))
    return NewNodeUnaryOp(ND_DEREF, unary());
  else if(Consume("+"))
    return unary();
  else if(Consume("-"))
     return NewNodeBinOp(ND_SUB, NewNodeNum(NULL, 0), unary());
  else if (Consume("++"))
    return NewNodeUnaryOp(ND_PREINC, unary());
  else if (Consume("--"))
    return NewNodeUnaryOp(ND_PREDEC, unary());
  else {
    Node *node = postfix();
    return node;
  }
}

Node *postfix() {
  Node *node = primary(), *subscript, *arg[6];
  size_t num_args;

  while(1) {
    if (Consume("(")) {
      if(node->kind != ND_IDENT) {
        ErrorAt(node->tok->str, "Identifier expected.");
      }
      num_args = 0;
      if(!Consume(")")) {
        arg[num_args++] = expr();
        while(num_args<=6) {
          if(Consume(",")) {
            arg[num_args++] = expr();
          } else {
            break;
          }
        }
        Expect(")");
      }
      node = NewNodeFuncCall(node->tok, num_args, arg);
    } else if(node->kind == ND_IDENT) {
      if(IsGlobalVar(node->tok)) {
        node = NewNodeGvar(node->tok);
      } else {
        node = NewNodeLvar(node->tok);
      } 
    } else if(Consume("[")) {
      subscript = expr();
      Expect("]");
      if(IsGlobalVar(node->tok)) {
        node = NewNodeUnaryOp(ND_DEREF,
          NewNodeBinOp(ND_ADD, node, subscript));
      } else {
        node = NewNodeUnaryOp(ND_DEREF,
          NewNodeBinOp(ND_ADD, node, subscript));
      }
    } else if (Consume("++")) {
      node = NewNodeUnaryOp(ND_POSTINC, node);
    } else if (Consume("--")) {
      node = NewNodeUnaryOp(ND_POSTDEC, node);
    } else {
      break;
    }
  }
  return node;
}

Node *primary() {
  Node *node;
  Token *tok;

  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (Consume("(")) {
    node = expr();
    Expect(")");
    return node;
  } else if(tok=ConsumeIdent()) {
    return NewNodeIdent(tok);
  } else if(node=string()) {
    return node;
  } else {
    return const_();
  }
}

Node *const_() {
  Token *tok;
  if (tok=ConsumeChar()) {
    return NewNodeChar(tok);
  } else { // else should be a number.
    return NewNodeNum(token, ExpectNumber());
  }
}

Node *string() {
  Token *tok;
  if (tok = ConsumeStrings()) {
    return NewNodeStrings(tok);
  } else {
    return NULL;
  }
}