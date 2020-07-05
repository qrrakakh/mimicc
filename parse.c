#include "mimicc.h"

//////////
// type-related functions
Type *InitArrayType(Type *_ty, size_t size) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_ARRAY;
  ty->ptr_to = _ty;
  ty->array_size = size;
  return ty;
}

_Bool IsArithmeticType(Type *ty) {
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
  Scope *scope = calloc(1, sizeof(Scope));
  scope->id = ++last_scope_id;
  scope->parent = current_scope;
  current_scope = scope;
}

void LeaveScope() {
  current_scope = current_scope->parent;
  if(is_look_ahead)
    --last_scope_id;
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

Token *ConsumeNumber() {
  if(token->kind != TK_NUM)
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

_Bool AtEOF() {
  return token->kind == TK_EOF;
}

////////
// ast-related functions

// find if the local var is already defined
_Bool IsParentOfScopeId(int id, Scope *scope) {
  if (scope->id==id) {
    return 1;
  } else if(scope->parent==NULL || scope->parent->id == 0) {
    return 0;
  } else {
    return IsParentOfScopeId(id, scope->parent);
  }
}

Symbol *FindLvar(Token *tok) {
  Symbol *symbol;
  for(symbol=locals;symbol;symbol=symbol->next) {
    if(symbol->kind != SY_VAR) continue;
    if(symbol->len == tok->len && !memcmp(symbol->name, tok->str, symbol->len)) {
      if(IsParentOfScopeId(symbol->scope_id, current_scope))
        return symbol;
    }
  }
  return NULL;
}

_Bool IsSymbolDefinedInScope(Token *tok, Symbol *head, int scope_id) {
  Symbol *s;
  for(s=head;s;s=s->next) {
    if(s->len == tok->len && !memcmp(s->name, tok->str, s->len) && s->scope_id == scope_id) {
      return 1;
    }
  }
  return 0;
}

Symbol *AddLvar(Token *tok, Type *ty) {
  Symbol *symbol;
  if(IsSymbolDefinedInScope(tok, locals, current_scope->id)) {
    ErrorAt(tok->str, "Already declared symbol is used in local variable declaration.");
  } else if(ty->kind == TYPE_VOID) {
    ErrorAt(tok->str, "Local variable declared void.");
  }
  symbol = calloc(1, sizeof(Symbol));
  symbol->next = locals; symbol->prev = NULL;
  locals->prev = symbol; locals = symbol; 
  symbol->kind = SY_VAR;
  symbol->name = tok->str;
  symbol->len = tok->len;
  symbol->ty = ty;
  symbol->scope_id = current_scope->id;
  symbol->id = ++last_symbol_id;
  
  if (ty->kind == TYPE_STRUCT) {
    symbol->struct_id = ty->id;
  }

  return symbol;
}

Symbol *FindGvar(Token *tok) {
  Symbol *symbol;
  for(symbol=globals;symbol;symbol=symbol->next) {
    if(symbol->kind != SY_VAR) continue;
    if(symbol->len == tok->len && !memcmp(symbol->name, tok->str, symbol->len)) {
      return symbol;
    }
  }
  return NULL;
}

Symbol *FindVarById(int id, Symbol *head) {
  Symbol *symbol;
  for(symbol=head;symbol;symbol=symbol->next) {
    if(symbol->id == id) {
      return symbol;
    }
  }
  return NULL;
}

Symbol *AddGVar(Token *tok, Type *ty, _Bool is_extern) {
  Symbol *symbol;
  if(IsSymbolDefinedInScope(tok, globals, current_scope->id)) {
    ErrorAt(tok->str, "Already declared symbol is used in global variable declaration.");
  } else if(ty->kind == TYPE_VOID) {
    ErrorAt(tok->str, "Global variable declared void.");
  }
  symbol = calloc(1, sizeof(Symbol));
  symbol->next = globals; symbol->prev = NULL;
  globals->prev = symbol; globals = symbol;
  symbol->kind = SY_VAR;
  symbol->name = tok->str;
  symbol->len = tok->len;
  symbol->ty = ty;
  if(is_extern) {
    symbol->scope_id = -1;
  } else {
    symbol->scope_id = 0;
  }

  symbol->id = ++last_symbol_id;

  if (ty->kind == TYPE_STRUCT) {
    symbol->struct_id = ty->id;
  }

  return symbol;
}

Symbol *FindLvarById(int id) {
  return FindVarById(id, locals);
}

Symbol *FindGvarById(int id) {
  return FindVarById(id, globals);
}

void RequiresLval(Node *node, char *s) {
  if (!(node->kind == ND_LVAR || node->kind == ND_GVAR
        || node->kind == ND_DEREF
        || node->kind == ND_ARROW || node->kind == ND_DOT)) {
    ErrorAt(s, "lvalue is expected, the kind is %d.", node->kind);
  }
}

Func *FindFuncByName(char *name, int name_len) {
  Func *f;
  for(f=funcs;f->next;f=f->next) {
    if(f->symbol->len == name_len && !memcmp(f->symbol->name, name, f->symbol->len)) {
      return f;
    }
  }
  return NULL;
}

Func *FindFunc(Token *tok) {
  return FindFuncByName(tok->str, tok->len);
}

Func *AddFunc(Token *tok, Type *ty, int num_args, int scope_id) {
  Func *f;
  Symbol *s;
  if(IsSymbolDefinedInScope(tok, globals, scope_id)) {
    ErrorAt(tok->str, "Already declared symbol is used in function declaration.");
  }

  s = calloc(1, sizeof(Symbol));
  s->next = globals; s->prev = NULL;
  globals->prev = s; globals = s;
  s->kind = SY_FUNC;
  s->name = tok->str;
  s->len = tok->len;
  s->ty = ty;
  s->id = ++last_symbol_id;

  f = calloc(1, sizeof(Func));
  f->symbol = s;
  f->next = funcs; funcs = f;
  f->num_args = num_args;
  return f;
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

Type *AddStruct(Token *tok, Symbol *members) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_STRUCT;
  ty->id = ++last_struct_id;

  Struct *s = calloc(1, sizeof(Struct));
  s->next = structs; structs = s;
  s->id = ty->id;
  s->ty = ty;
  s->scope_id = current_scope->id;
  s->members = members;

  if(tok) {
    s->name = tok->str;
    s->len = tok->len;
  } else {
    s->name = NULL;
    s->len = 0;
  }

  return ty;
}

Symbol *AddEnumConst(int id, int val, Token *tok) {
  Type *ty = calloc(1, sizeof(Type));
  ty->id = id;
  ty->kind = TYPE_ENUM;
  
  Symbol *s = calloc(1, sizeof(Symbol));
  if(current_scope->id==0) {
    s->next = globals; s->prev = NULL;
    globals->prev = s; globals = s;
  }
  else {
    s->next = locals; s->prev = NULL;
    locals->prev = s; locals = s;
  }
  s->kind = SY_ENUMCONST;
  s->name = tok->str;
  s->len = tok->len;
  s->ty = ty;
  s->id = ++last_symbol_id;
  s->scope_id = current_scope->id;
  s->struct_id = id;
  s->val = val;

  return s;
}

Type *AddEnum(Token *tok) {
  Type *ty = calloc(1, sizeof(Type));
  ty->kind = TYPE_ENUM;
  ty->id = ++last_enum_id;

  Enum *e = calloc(1, sizeof(Enum));
  e->next = enums; enums = e;
  e->id = ty->id;
  e->ty = ty;
  e->scope_id = current_scope->id;
  
  if(tok) {
    e->name = tok->str;
    e->len = tok->len;
  } else {
    e->name = NULL;
    e->len = 0;
  }

  return ty;
}

Struct *FindStruct(Token *tok, _Bool is_recursive_search) {
  Struct *s = structs;
  while(s->next) {
    if (tok->len == s->len
        && strncmp(s->name, tok->str, s->len)==0) {
      if(is_recursive_search && (IsParentOfScopeId(s->scope_id, current_scope) || s->scope_id==0)) {
        return s;
      } else if((!is_recursive_search) && s->scope_id==current_scope->id) {
        return s;
      }
    }
    s = s->next;
  }
  return NULL;
}

Struct *FindStructById(int struct_id) {
  Struct *s;
  for(s=structs;s->next && s->id != struct_id;s=s->next) ;
  if(!(s->next)) {
    Error("Struct not found, %d.\n", struct_id);
  }
  return s;
}

Symbol *FindStructMember(int struct_id, Token *member_token) {
  Symbol *v;
  Struct *s = FindStructById(struct_id);
  for(v=s->members;
      v->next!=NULL &&
      (v->len != member_token->len || strncmp(v->name, member_token->str, v->len)!=0);
      v=v->next);
  if(!v->next) return NULL;
  return v;
}

Enum *FindEnum(Token *tok, _Bool is_recursive_search) {
  Enum *e = enums;
  while(e->next) {
    if (tok->len == e->len
        && strncmp(e->name, tok->str, e->len)==0) {
      if(is_recursive_search && (IsParentOfScopeId(e->scope_id, current_scope) || e->scope_id==0)) {
        return e;
      } else if((!is_recursive_search) && e->scope_id==current_scope->id) {
        return e;
      }
    }
    e = e->next;
  }
  return NULL;
}

Enum *FindEnumById(int enum_id) {
  Enum *e;
  for(e=enums;e->next && e->id != enum_id;e=e->next) ;
  if(!(e->next)) {
    Error("Enum not found, %d.\n", enum_id);
  }
  return e;
}

_Bool IsGlobalVar(Token *tok) {
  if(FindLvar(tok))
    return 0;
  if(FindGvar(tok))
    return 1;
  else 
    return 0;
}

Symbol *FindConstSymbol(Token *tok) {
  Symbol *symbol;

  // find local variables
  for(symbol=locals;symbol;symbol=symbol->next) {
    if(symbol->kind != SY_ENUMCONST) continue;
    if(symbol->len == tok->len && !memcmp(symbol->name, tok->str, symbol->len)) {
      if(IsParentOfScopeId(symbol->scope_id, current_scope))
        return symbol;
    }
  }

  // find global variables
  for(symbol=globals;symbol;symbol=symbol->next) {
    if(symbol->kind != SY_ENUMCONST) continue;
    if(symbol->len == tok->len && !memcmp(symbol->name, tok->str, symbol->len)) {
      return symbol;
    }
  }

  return NULL;
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
      node->ty = int_type;
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
      node->ty = int_type;
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

Node *NewNodeArrow(Node *lhs, Token *member_token) {
  Symbol *v;
  if(lhs->ty->kind != TYPE_PTR || lhs->ty->ptr_to->kind != TYPE_STRUCT) {
    ErrorAt(token->str, "Left hand of the arrow operater must be a pointer of the struct.");
  }

  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_ARROW;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = lhs;

  if(!(v = FindStructMember(lhs->ty->ptr_to->id , member_token)))
    ErrorAt(member_token->str, "member is not found.");
  node->id = v->id;
  node->tok = NULL;
  node->ty = v->ty;
  
  return node;
}

Node *NewNodeDot(Node *lhs, Token *member_token) {
  Symbol *v;
  if(lhs->ty->kind != TYPE_STRUCT) {
    ErrorAt(token->str, "Left hand of the dot operater must be a struct");
  }

  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_DOT;
  node->children = calloc(1, sizeof(Node*));
  node->children[0] = lhs;

  if(!(v = FindStructMember(lhs->ty->id , member_token)))
    ErrorAt(member_token->str, "member is not found.");
  node->id = v->id;
  node->tok = NULL;
  node->ty = v->ty;
  
  return node;
}

Node *NewNodeChar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_CHAR;
  node->val = *(tok->str);
  node->tok = tok;
  node->ty = char_type;
  return node;
}

Node *NewNodeStrings(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_STRINGS;
  node->id = FindCstr(tok->str, tok->len)->id;
  node->tok = tok;
  node->ty = InitArrayType(char_type, tok->len);
  return node;
}

Node *NewNodeNum(Token *tok, int val) {
  Node *node = calloc(1, sizeof(Node));
  node->children = NULL;
  node->kind = ND_NUM;
  node->val = val;
  node->ty = int_type;
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
  Symbol *symbol;

  if(!(symbol=FindLvar(tok))) {
    ErrorAt(tok->str, "Undefined local variable.");
  }

  node->children = NULL;
  node->kind = ND_LVAR;
  node->id = symbol->id;
  node->tok = tok;
  if (symbol->ty->kind == TYPE_ENUM)
    node->ty = int_type;
  else 
    node->ty = symbol->ty;
  return node;
}

Node *NewNodeGvar(Token *tok) {
  Node *node = calloc(1, sizeof(Node));
  Symbol *symbol;
  
  if(!(symbol=FindGvar(tok))) {
    ErrorAt(tok->str, "Undefined global variable.");
  }
  node->children=NULL;

  node->kind = ND_GVAR;
  node->id = symbol->id;
  node->tok = tok;
  if (symbol->ty->kind == TYPE_ENUM)
    node->ty = int_type;
  else 
    node->ty = symbol->ty;
  return node;
}

Node *NewNodeConstInt(Token *tok) {
  Node *node;
  Symbol *symbol;
  
  if (!(symbol=FindConstSymbol(tok)))
    return NULL;

  node = calloc(1, sizeof(Node));
  node->children=NULL;
  node->kind = ND_NUM;
  node->id = symbol->id;
  node->tok = tok;
  node->val = symbol->val;
  if (symbol->ty->kind == TYPE_ENUM)
    node->ty = int_type;
  else 
    node->ty = symbol->ty;

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
  Func *f;
  if(!is_look_ahead && !(f = FindFunc(tok))) {
    WarnAt(tok->str, "Implicitly declared function.");
    f = AddFunc(tok, int_type, 0, current_scope->id);
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
  node->ty = f->symbol->ty;
  return node;
}

// Non-terminal symbols generator
void program();
Node *func(_Bool is_extern);
Node *block();
Node *stmt();
Type *type();
Type *struct_();
Type *enum_();
void enumerator_list(int id);
int enumerator(int id, int my_val);
Node *declare();
Node *declare_a();
void declare_e();
void evar(Type *_ty);
Node *var_a(Type *_ty);

// Expression
Node *expression();
Node *assignment_expression();
Node *unary_expression();
Node *cast_expression();
Node *postfix_expression();
Node *additive_expression();
Node *multiplicative_expression();
Node *shift_expression();
Node *relational_expression();
Node *equality_expression();
Node *and_expression();
Node *xor_expression();
Node *or_expression();
Node *logical_and_expression();
Node *logical_or_expression();
Node *conditional_expression();

// Primary expression / constants
Node *primary_expression();
Node *identifier();
Node *constant();
Node *integer_constant();
Node *enumeration_constant();
Node *character_constant();
Node *string_literal();

void program() {
  int i=0;
  int alloc_unit = 10;
  int alloc_size = alloc_unit;

  Token *_tok;
  Node *node;
  Type *ty, *tgt_ty;

  globals = calloc(1, sizeof(Symbol)); 
  globals->next = NULL;
  globals->id = 0;

  cstrs = calloc(1, sizeof(Const_Strings));
  cstrs->next = NULL;
  cstrs->id = 0;

  funcs = calloc(1, sizeof(Func));
  funcs->next = NULL;

  structs = calloc(1, sizeof(Struct));
  structs->next = NULL;
  structs->id = 0;
  last_struct_id = 0;

  enums = calloc(1, sizeof(Enum));
  enums->next = NULL;
  enums->id = 0;
  last_enum_id = 0;

  last_symbol_id = 0;
  last_scope_id = 0;
  ctrl_depth = 0;
  is_look_ahead = 0;

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

    is_look_ahead = 1;
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
        is_look_ahead = 0;
        node = func(1);
        Expect(";");
      } else if(Consume("[") ||Consume(",") || Consume(";")) { // global variable
        token = _tok;
        is_look_ahead = 0;
        declare_e();
        Expect(";");
      } else {
        token = _tok;
        is_look_ahead = 0;
        ErrorAt(token->str, "Definition of the global variable or the function should be allowed.");
      }
      continue;
    } else {
      ty = type();
      while(Consume("*"));
      if(!ConsumeIdent()) {
        ErrorAt(token->str, "Invalid definition statement.");
      }
      if(Consume("(")) { // func
        token = _tok;
        is_look_ahead = 0;
        codes[i++] = func(0);
      } else if(Consume("[") ||Consume(",") || Consume(";")) { // global variable
        token = _tok;
        is_look_ahead = 0;
        codes[i++] = declare_a();
        Expect(";");
      } else {
        token = _tok;
        is_look_ahead = 0;
        ErrorAt(token->str, "Definition of the global variable or the function should be allowed.");
      }
    }
  }
  codes[i] = NULL;
}

Node *func(_Bool is_extern) {
  Token *type_tok, *ident_tok, *arg_tok;
  Type *ty, *tgt_ty;
  Node *arg[6], *block_node = NULL;
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
  locals = calloc(1, sizeof(Symbol)); 
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

  if(!is_look_ahead)
    current_func=AddFunc(ident_tok, ty, num_args, current_scope->parent->id);

  if(!is_extern) {
    block_node = block();
  }
  LeaveScope();

  return NewNodeFunc(ident_tok, ty, num_args, block_node);
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
    if (current_func->symbol->ty->kind == TYPE_VOID) {
      if(Consume(";")) {
        node = NewNodeUnaryOp(ND_RETURN, NULL);
      } else {
        WarnAt(tok->str, "Returned with a value in void function.");
        node = NewNodeUnaryOp(ND_RETURN, expression());
        Expect(";");
      }
    } else {
      if(tok = Consume(";")) {
        ErrorAt(tok->str, "Returned without a value in non-void function.");
      } else {
        node = NewNodeUnaryOp(ND_RETURN, expression());
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
  } else if(node = declare_a()) {
    Expect(";");
  } else if (node = block()) {
    return node;
  } else if(tok = Consume("while")) {
    Expect("(");
    Node *cond = expression();
    Expect(")");
    ++ctrl_depth;
    node = NewNodeBinOp(ND_WHILE, cond, stmt());
    --ctrl_depth;
  } else if(tok = Consume("for")) {
    EnterScope();
    Expect("(");
    Node *init;
    if(!(init=declare_a())) {
      init = expression();
    }
    Expect(";");
    Node *cond = expression();
    Expect(";");
    Node *next = expression();
    Expect(")");
    ++ctrl_depth;
    node = NewNodeFor(init, cond, next, stmt());
    --ctrl_depth;
    LeaveScope();
  } else if(tok = Consume("if")) {
    Expect("(");
    Node *cond = expression();
    Expect(")");
    Node *stmt1 = stmt();
    Node *stmt2 = NULL;
    if (tok = Consume("else")) {
      stmt2 = stmt();
    }
    node = NewNodeIf(cond, stmt1, stmt2);
  } else if(tok = Consume("switch")) {
    Expect("(");
    Node *cond = expression();
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
    Node *cond = expression();
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
    node = expression();
    Expect(";");
  }
  
  return node;
}

Type *struct_() {
  Token *tok, *ident_tok;
  Symbol *_locals;
  Node *node;
  Struct *s;
  Type *ty;
  if (!(tok=Consume("struct"))) {
    return NULL;
  }
  ident_tok = ConsumeIdent();

  if(Consume("{")) {
    if(ident_tok && FindStruct(ident_tok, 0)) {
      ErrorAt(ident_tok->str, "struct is already declared.");
    }

    _locals = locals;
    locals = calloc(1, sizeof(Symbol));
    locals->next = NULL;
    locals->id = 0;

    EnterScope();
    while(!Consume("}")) {
      node = declare_a();
      Expect(";");
    }
    LeaveScope();
    if (!is_look_ahead)
      ty = AddStruct(ident_tok, locals);
    locals = _locals;

    return ty;
  } else if(!ident_tok) {
    ErrorAt(token->str, "identifier expected.");
  } else if (!(s = FindStruct(ident_tok, 1))) {
    ErrorAt(ident_tok->str, "struct is not declared.");
  } else {
    return s->ty;
  }
}

Type *type() {
  Type *ty;
  Token *tok;
  if(ty = struct_()) {
    return ty;
  } else if(ty = enum_()) {
    return ty;
  } else if(!(tok = ConsumeTypeStr())) {
    return NULL;
  }
  for(int i=0;i<num_builtin_types;++i) {
    if(strncmp(tok->str, builtin_type_names[i], tok->len)==0) {
      ty = builtin_type_obj[i];
    }
  }

  return ty;
}

Type *enum_() {
  Token *tok, *ident_tok;
  Enum *e;
  Type *ty;
  if (!(tok=Consume("enum"))) {
    return NULL;
  }
  ident_tok = ConsumeIdent();

  if(Consume("{")) {
    if(ident_tok && FindEnum(ident_tok, 0)) {
      ErrorAt(ident_tok->str, "enum is already declared.");
    }

    if (!is_look_ahead)
      ty = AddEnum(ident_tok);

    enumerator_list(ty->id);
    Consume(",");
    Expect("}");

    return ty;
  } else if(!ident_tok) {
    ErrorAt(token->str, "identifier expected.");
  } else if (!(e = FindEnum(ident_tok, 1))) {
    ErrorAt(ident_tok->str, "enum is not declared.");
  } else {
    return e->ty;
  }
}

void enumerator_list(int id) {
  int val = enumerator(id, 0);
  ++val;
  while(Consume(",")) {
    val = enumerator(id, val);
    ++val;
  }
}

int enumerator(int id, int my_val) {
  Token *ident_token;
  ident_token = ConsumeIdent();
  if(Consume("=")) {
    my_val = ExpectNumber();
  }
  if (!is_look_ahead)
    AddEnumConst(id, my_val, ident_token);
  return my_val;
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

  if(!is_look_ahead)
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

  if(!is_look_ahead) {
    if (current_scope->id==0) {
      AddGVar(ident_tok, ty, 0);
    } else {
      AddLvar(ident_tok, ty);
    }
  }

  if (Consume("=")) {
    assign_node = NewNodeBinOp(ND_ASSIGN, NewNodeLvar(ident_tok), assignment_expression());
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

  if(!is_look_ahead)
    AddGVar(tok, ty, 1);
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

Node *expression() {
  // expression = assignment-expression
  //              | expression "," assignment-expression     <<<<< not implemented
  return assignment_expression();
}

Node *assignment_expression() {
  // assignment-expression = conditional-expression
  //                       | unary-expression assignment-operator assignment-expression
  // assignment-operator = "=" | "*=" | "/=" | "%=" | "+=" | "-="
  //                     | "<<=" | ">>=" | "&=" | "^=" | "|=" ## not implemented
  Token *tok = token;
  Node *node;
  
  // Node is evaluated as conditional_expression once.
  // but it is required to be unary-expression syntactically; more specifically to be lvalue
  // if assignment-operator appears.
  node = conditional_expression();

  for(;;) {
    if(Consume("=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, assignment_expression());
    } else if(Consume("+=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_ADD, node, assignment_expression()));
    } else if(Consume("-=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_SUB, node, assignment_expression()));
    } else if(Consume("*=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_MUL, node, assignment_expression()));
    } else if(Consume("/=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_DIV, node, assignment_expression()));
    } else if(Consume("\%=")) {
      RequiresLval(node, tok->str);
      node = NewNodeBinOp(ND_ASSIGN, node, NewNodeBinOp(ND_MOD, node, assignment_expression()));
    } else {  // conditional-expression (including unary-expression without assignment)
      return node;
    }
  }
}

Node *and_expression() {
  // AND-expression = equality-expression
  //                | AND-expression "&" equality-expression ## not implemented
  return equality_expression();
}

Node *xor_expression() {
  // XOR-expression = AND-expression
  //                  | XOR-expression "^" AND-expression ## not implemented
  return and_expression();
}

Node *or_expression() {
  // OR-expression = XOR-expression
  //                 | OR-expression "|" XOR-expression ## not implemented
  return xor_expression();
}

Node *logical_and_expression() {
  // logical-AND-expression = OR-expression
  //                          | logical-AND-expression "&&" XOR-expression ## not implemented
  return or_expression();
}

Node *logical_or_expression() {
  // logical-OR-expression = logical-AND-expression
  //                         | logical-OR-expression "||" logical-AND-expression ## not implemented
  return logical_and_expression();
}

Node * conditional_expression() {
  // conditional-expression = logical-OR-expression
  //                          | logical-OR-expression "?" expression ":" conditional-expression ## not implemented  
  return logical_or_expression();
}


Node *equality_expression() {
  // equality-expression = relational-expression
  //                     | equality-expression "==" relational-expression
  //                     | equality-expression "!=" relational-expression
  Node *node = relational_expression();

  for(;;) {
    if (Consume("=="))
      node = NewNodeBinOp(ND_EQUIV, node, relational_expression());
    else if (Consume("!="))
      node = NewNodeBinOp(ND_INEQUIV, node, relational_expression());
    else
      return node;
  }
}

Node *shift_expression() {
  // shift-expression = additive-expression
  //                    | shift-expression "<<" additive-expression ## not implemented
  //                    | shift-expression ">>" additive-expression ## not implemented  
  return additive_expression();
}

Node *relational_expression() {
  // relational-expression = shift-expression
  //                       | relational-expression "<" shift-expression
  //                       | relational-expression ">" shift-expression
  //                       | relational-expression "<=" shift-expression
  //                       | relational-expression ">=" shift-expression
  Node *node = shift_expression();

  for(;;) {
    if (Consume("<="))
      node = NewNodeBinOp(ND_LE, node, additive_expression());
    else if (Consume(">="))
      node = NewNodeBinOp(ND_LE, additive_expression(), node);
    else if (Consume("<"))
      node = NewNodeBinOp(ND_LT, node, additive_expression());
    else if (Consume(">"))
      node = NewNodeBinOp(ND_LT, additive_expression(), node);
    else
      return node;
  }
}

Node *additive_expression() {
  // additive-expression = multiplicative-expression
  //                     | additive-expression "+" multiplicative-expression
  //                     | additive-expression "-" multiplicative-expression
  Node *node = multiplicative_expression();

  for(;;) {
    if (Consume("+"))
      node = NewNodeBinOp(ND_ADD, node, multiplicative_expression());
    else if (Consume("-"))
      node = NewNodeBinOp(ND_SUB, node, multiplicative_expression());
    else
      return node;
  }
}

Node *multiplicative_expression() {
  // multiplicative-expression = cast-expression
  //                           | multiplicative-expression "*" cast-expression
  //                           | multiplicative-expression "/" cast-expression
  //                           | multiplicative-expression "%" cast-expression
  Node *node = cast_expression();

  for(;;) {
    if(Consume("*"))
      node = NewNodeBinOp(ND_MUL, node, cast_expression());
    else if(Consume("/"))
      node = NewNodeBinOp(ND_DIV, node, cast_expression());
    else if(Consume("\%"))
      node = NewNodeBinOp(ND_MOD, node, cast_expression());
    else
      return node;
  }
}

Node *unary_expression() {
  // unary-expression = postfix-expression
  //                    | ++ unary-expression
  //                    | -- unary-expression
  //                    | unary-operator cast-expression
  //                    | "sizeof" unary-expression
  //                    | sizeof "(" type-name ")"  ## not implemented  
  if (Consume("sizeof"))
    return NewNodeUnaryOp(ND_SIZEOF, unary_expression());
  // unary-operators
  else if(Consume("&"))
    return NewNodeUnaryOp(ND_ADDR, cast_expression());
  else if(Consume("*"))
    return NewNodeUnaryOp(ND_DEREF, cast_expression());
  else if(Consume("+"))
    return cast_expression();
  else if(Consume("-"))
     return NewNodeBinOp(ND_SUB, NewNodeNum(NULL, 0), cast_expression());
  // ++ / --
  else if (Consume("++"))
    return NewNodeUnaryOp(ND_PREINC, unary_expression());
  else if (Consume("--"))
    return NewNodeUnaryOp(ND_PREDEC, unary_expression());
  else {
    Node *node = postfix_expression();
    return node;
  }
}

Node *cast_expression() {
  return unary_expression();
}

Node *postfix_expression() {
  // postfix-expression = primary-expression
  //                    | postfix-expression "[" expression "]"
  //                    | postfix-expression "(" argument-expression-list? ")"
  //                    | postfix-expression "." identifier
  //                    | postfix-expression "->" identifier
  //                    | postfix-expression "++"
  //                    | postfix-expression "--"
  //                    | "(" type-name ")" "{" initializer-list ","? "}" ## not implemented
  Node *node = primary_expression(), *_node, *subscript, *arg[6];
  size_t num_args;

  while(1) {
    if (Consume("(")) {  // postfix-expression "(" argument-expression-list? ")"
      if(node->kind != ND_IDENT) {
        ErrorAt(node->tok->str, "Identifier expected.");
      }
      num_args = 0;
      if(!Consume(")")) {
        arg[num_args++] = expression();
        while(num_args<=6) {
          if(Consume(",")) {
            arg[num_args++] = expression();
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
    } else if(Consume("[")) { // postfix-expression "[" expression "]"
      subscript = expression();
      Expect("]");
      node = NewNodeUnaryOp(ND_DEREF,
        NewNodeBinOp(ND_ADD, node, subscript));
    } else if (Consume("++")) { // postfix-expression "++"
      node = NewNodeUnaryOp(ND_POSTINC, node);
    } else if (Consume("--")) { // postfix-expression "--"
      node = NewNodeUnaryOp(ND_POSTDEC, node);
    } else if (Consume("->")) { // postfix-expression "->" identifier
      node = NewNodeArrow(node, ConsumeIdent());
    } else if (Consume(".")) { // postfix-expression "." identifier
      node = NewNodeDot(node, ConsumeIdent());
    } else {
      break;
    }
  }
  return node;
}

Node *primary_expression() {
  // primary-expression = identifier
  //                    | constant
  //                    | string-literal
  //                    | "(" expression ")"

  Node *node;
  // if the next token is '(' then it should be expanded as '(' expr ')'
  if (Consume("(")) { // "(" expression ")"
    node = expression();
    Expect(")");
    return node;
  } else if(node=constant()) { // constant
  // evaluate earlier than identifier so that enum const must be parsed as a constant.
    return node;
  } else if(node=string_literal()) { // string-literal
    return node;
  } else {  // identifier
    return identifier();
  }
}

Node *identifier() {
  Token *tok;
  if (tok=ConsumeIdent()) {
    return NewNodeIdent(tok);
  } else {
    return NULL;
  }
}

Node *constant() {
  // constant = integer-constant
  //           | floating-constant ## not implemented
  //           | enumeration-constant
  //           | character-constant
  
  Node *node;
  if (node=integer_constant()) { // integer_constant
    return node;
  } else if(node=enumeration_constant()) {
    return node;
  } else { // character-constant
    return character_constant();
  }
}

Node *integer_constant() {
  // integer-constant = decimal-constant ## currently restricted to int integer

  Token *tok = ConsumeNumber();
  if(tok) {
    return NewNodeNum(tok, tok->val);
  } else {
    return NULL;
  }
}

Node *enumeration_constant() {
  // enumeration-constant = identifier
  Token *tok, *_tok;
  Node *node;
  _tok = token;
  if(!(tok=ConsumeIdent())) {
    return NULL;
  } else if(node = NewNodeConstInt(tok)) {
    return node;
  } else {
    token = _tok;
    return NULL;
  }
}

Node *character_constant() {
  // character-constant = ''' c-char-sequence '''
  //                     | 'L'' c-char-sequence ''' ## not implemented
  // c-char-sequence = c-char-sequence? c-char

  // currently c-char-sequence is parsed by lexer.

  Token *tok = ConsumeChar();
  if(tok) {
    return NewNodeChar(tok);
  } else {
    return NULL;
  }
}

Node *string_literal() {
  // string-literal = '"' s-char-sequence? '"'
  //                 | 'L'' s-char-sequence? ''' ## not implemented
  // s-char-sequence = s-char-sequence? c-char  ## currently resticted that c-char is equal to s-char

  // currently s-char-sequence is parsed by lexer.

  Token *tok;
  if (tok = ConsumeStrings()) {
    return NewNodeStrings(tok);
  } else {
    return NULL;
  }
}