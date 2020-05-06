#include "9cc.h"

//////////
// static value
char x86_64_argreg[][6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

// debug function
void print_node(Node* node, int offset) {
  fprintf(stderr, "%*s---\n",offset*4, "");
  fprintf(stderr, "%*skind: %d\n",offset*4,"", node->kind);
  fprintf(stderr, "%*sval: %d\n",offset*4,"",node->val);
  fprintf(stderr, "%*soffset: %d\n",offset*4,"",node->offset);
  fprintf(stderr, "%*sfunc_name: %s\n",offset*4,"",node->func_name);
  int num_children = 0;
  if (node->kind==ND_CALL) {
    num_children = node->offset;
  } else if(node->kind==ND_RETURN || node->kind == ND_ADDR || node->kind == ND_DEREF || node->kind == ND_FUNC) {
    // unary op / func
    num_children = 1;
  }  else if(node->kind==ND_WHILE || node->kind == ND_ASSIGN || node->kind == ND_EQUIV || node->kind == ND_INEQUIV
            || node->kind == ND_LE || node->kind == ND_LT
            || node->kind == ND_ADD || node->kind == ND_SUB || node->kind == ND_MUL || node->kind == ND_DIV
            || node->kind == ND_MOD ) {
    // bin op
    num_children = 2;
  }  else if(node->kind==ND_IF) {
    num_children = 3;
    if(!(*(node->children+2))) {
      num_children = 2;
    }
  }  else if(node->kind==ND_FOR) {
    num_children = 4;
  } else if(node->kind==ND_BLOCK) {
    int i;
    for(i=0;node->children[i];i++){;}
    num_children = i;
  }

  for(int i=0;i<num_children;++i) {
    print_node(*(node->children+i), offset+1);
  }
}

void print_node_tree() {
  for(int i=0;code[i];i++) {
    print_node(code[i], 0);
  }
}

// type helper function
int size_ptr(Type* ty) {
  if (ty->ptr_to->ty == TYPE_INT) {
    return 4;
  } else {
    return 8;
  }
}

// Code generator
void gen_lval(Node *node) {
  if (node->kind == ND_LVAR) {
    // save the address of lval
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
  } else if(node->kind == ND_DEREF) {
    gen(node->children[0]);
  } else {
    error("lval is not a variable");
  }
}


void gen(Node *node) {
  int label;
  Node **stmt_list;
  Type *lhs_ty, *rhs_ty;
  LVar *var;

  switch(node->kind) {
    case ND_FUNC:
    printf("%.*s:\n", node->val, node->func_name);

    locals = node->lvars;
    // allocate stack for local variables
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", get_num_lvars() * 8);

    // copy passed argument values to the local variables
    for(int i=0;i<node->offset;++i) {
      printf("  mov [rbp-%d], %s\n", i*8+8, x86_64_argreg[i]);
    }


    gen(node->children[0]);
    printf("  pop rax\n");  

    // Final evaluated value is already stored on rax, which will be returned.
    // To back to the original address, we fix the rsp
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");

    return;

    case ND_RETURN:
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;

    case ND_BLOCK:
    stmt_list = node->children;
    if (*stmt_list != NULL)
      gen(*(stmt_list++));
    while (*stmt_list != NULL) {
      printf("  pop rax\n");  
      gen(*(stmt_list++));
    }
    return;

    case ND_ADDR:
    gen_lval(node->children[0]);
    current_type = calloc(1, sizeof(Type));
    current_type->ty = TYPE_PTR;
    current_type->ptr_to = find_lvar_by_offset(node->children[0]->offset)->ty;
    return;

    case ND_DEREF:
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    var = find_lvar_by_offset(node->children[0]->offset);
    if(!var) {
      error("lvar not found.");
    }
    if(!(var->ty)) {
      error("lvar type not found, offset: %d", node->children[0]->offset);
    }
    current_type = var->ty->ptr_to;
    return;

    case ND_NUM:
    printf("  push %d\n", node->val);
    current_type = calloc(1, sizeof(Type));
    current_type->ty = TYPE_INT;
    return;

    case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    current_type = find_lvar_by_offset(node->offset)->ty;
    return;

    case ND_ASSIGN:
    gen_lval(node->children[0]);
    gen(node->children[1]);
    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;

    case ND_WHILE:
    label = label_index++;
    printf(".L.begin%06d:\n", label);
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_FOR:
    label = label_index++;
    gen(node->children[0]);
    printf(".L.begin%06d:\n", label);
    gen(node->children[1]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L.end%06d\n", label);
    gen(node->children[3]);
    gen(node->children[2]);
    printf("  jmp .L.begin%06d\n", label);
    printf(".L.end%06d:\n", label);
    printf("  push 0\n");
    return;

    case ND_IF:
    label = label_index++;
    gen(node->children[0]);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n"); // 0 if cond is not satisfied
    printf("  je .L.else%06d\n", label);
    gen(node->children[1]);
    printf("  jmp .L.end%06d\n",  label);
    printf(".L.else%06d:\n",  label);
    if(node->children[2]!=NULL) {
      gen(node->children[2]);
    }
    printf(".L.end%06d:\n",  label);
    printf("  push 0\n");

    return;

    case ND_CALL:
    for(int i=0;i<node->offset;++i) {
      gen(node->children[i]);
    }

    for(int i=node->offset-1;i>=0;--i) {
      printf("  pop %s\n", x86_64_argreg[i]);
    }

    // modify rsp as a multiply of 16
    label = label_index++;
    printf("  mov rax, rsp\n");
    printf("  and rax, 15\n");
    printf("  jnz .L.call%06d\n", label);
    printf("  mov rax, 0\n");
    printf("  call %.*s\n", node->val, node->func_name);
    printf("  jmp .L.end%06d\n", label);
    printf(".L.call%06d:\n", label);
    printf("  sub rsp, 8\n");
    printf("  mov rax, 0\n");
    printf("  call %.*s\n", node->val, node->func_name);
    printf("  add rsp, 8\n");
    printf(".L.end%06d:\n", label);
    printf("  push rax\n");

    // currently assumed that function only returns TYPE_INT
    current_type = calloc(1, sizeof(Type));
    current_type->ty = TYPE_INT;
    return;
  }

  gen(node->children[0]);
  lhs_ty = current_type;
  gen(node->children[1]);
  rhs_ty = current_type;

  printf("  pop rdi\n");
  printf("  pop rax\n");

  // add
  // support type: INT/INT, INT/PTR, PTR/INT
  if(node->kind == ND_ADD) {
    if(lhs_ty->ty == TYPE_PTR && rhs_ty->ty == TYPE_PTR) {
      error("adding pointer to pointer is not valid.");
    }
    if(lhs_ty->ty == TYPE_PTR) {
      printf("  imul rdi, %d\n",size_ptr(lhs_ty));
    } else if(rhs_ty->ty == TYPE_PTR) {
      printf("  imul rax, %d\n",size_ptr(rhs_ty));
    }
    printf("  add rax, rdi\n");
  }

  // sub
  // support type: INT/INT, PTR/INT (not INT/PTR)
  if(node->kind == ND_SUB) {
    if(rhs_ty->ty == TYPE_PTR) {
      error("subtracting pointer is not valid.");
    }
    if(lhs_ty->ty == TYPE_PTR) {
      printf("  imul rdi, %d\n",size_ptr(lhs_ty));
    }
    printf("  sub rax, rdi\n");
  }  


  // compare operator
  // support type: INT/INT
  if((node->kind == ND_EQUIV || node->kind == ND_INEQUIV || node->kind == ND_LT || node->kind == ND_LE)
      && (lhs_ty->ty != rhs_ty->ty)) {
    error("different type cannot be compared; lhs type: %d, rhs type: %d, operation: %d",
          lhs_ty->ty, rhs_ty->ty, node->kind);
  }
  switch(node->kind) {
    case ND_EQUIV:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_INEQUIV:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  // mul/div
  // support type: INT/INT
  if((node->kind == ND_MUL || node->kind == ND_DIV || node->kind == ND_MOD)
      && (lhs_ty->ty != TYPE_INT || rhs_ty->ty != TYPE_INT)) {
    error("non-int binary operation for mul/div/mod is not supported; lhs type: %d, rhs type: %d, operation: %d",
          lhs_ty->ty, rhs_ty->ty, node->kind);
  }
  switch(node->kind) {
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
    case ND_MOD:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      printf("  mov rax, rdx\n");
      break;
  }

  printf("  push rax\n");
}
