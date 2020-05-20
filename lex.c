#include "9cc.h"

//////////
// tokenizer and helpers

// Generate new token and concatenate to cur
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
        || *p == '[' || *p == ']'
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

