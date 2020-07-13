#include "mimicc.h"

//////////
// tokenizer and helpers

char *builtin_type_names[] = {"int", "char", "void", "_Bool"};
TypeKind builtin_type_enum[] = {TYPE_INT, TYPE_CHAR, TYPE_VOID, TYPE_BOOL};
int num_builtin_types = 4;

char *reserved_keywords[] = {
  "auto",
  "break",
  "case",
  "char",
  "const",
  "continue",
  "default",
  "do",
  "double",
  "else",
  "enum",
  "extern",
  "float",
  "for",
  "goto",
  "if",
  "inline",
  "int",
  "long",
  "register",
  "restrict",
  "return",
  "short",
  "signed",
  "sizeof",
  "static",
  "struct",
  "switch",
  "typedef",
  "union",
  "unsigned",
  "void",
  "volatile",
  "while",
  "_Bool",
  "_Complex",
  "_Imaginary",
};
int num_reserved_keywords = 37;

_Bool IsNondigitChar(int p) {
  if (('a' <= p && p <= 'z') || ('A' <= p && p <= 'Z') || p == '_') {
    return 1;
  } else {
    return 0;
  }
}

_Bool IsIdentChar(int p) {
  if (IsNondigitChar(p) || isdigit(p)) {
    return 1;
  } else {
    return 0;
  }
}

_Bool IsKeyword(char *p, char *keyword) {
  int len = strlen(keyword);
  return strncmp(p, keyword, len)==0 && p+len && (!IsIdentChar(*(p+len)));
}

// Generate new token and concatenate to cur
Token *NewToken(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

Token *NewReservedToken(char *keyword, Token *cur, char **str) {
  int len = strlen(keyword);
  Token *tok;
  if(IsKeyword(*str, keyword)) {
    tok = NewToken(TK_RESERVED, cur, *str, len);
    *str += len;
    return tok;
  }
  return NULL;
}

// Tokenize input string p
Token *Tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head, *tok;
  int l;

  while (*p) {
    // skip blank
    if (isspace(*p)) {
      p++;
      continue;
    }

    // skip line comment
    if (strncmp(p, "//", 2)==0 ) {
      p += 2;
      while (*p != '\n')
        p++;
      continue;
    }

    // skip block comment
    if (strncmp(p, "/*", 2)==0) {
      char *q = strstr(p+2, "*/");
      if (!q) {
        ErrorAt(p, "Block comment is not closed.");
      }
      p = q + 2;
      continue;
    }

    // reserved keywords
    for(int i=0;i<num_reserved_keywords;++i) {
      if((tok=NewReservedToken(reserved_keywords[i], cur, &p))) {
        break;
      }
    }
    if(tok) {
      cur=tok; continue;
    }

    // identifier
    if (IsNondigitChar(*p)) {
      for(l=1;(p+l)&&(IsIdentChar(*(p+l)));++l);
      cur = NewToken(TK_IDENT, cur, p, l);
      p+=l;
      continue;
    }

    // punctuators
    // Todo: implement
    // ~, !, 
    // <<, >>, ^, |, &&, ||, 
    // ?, ..., 
    // <<=, >>=, &=, ^=, |=,
    // #, ##,
    // <:, :>, <%, %>, %:, %:%:

    // two chars punctuator
    if (strlen(p) >= 2) {
      if( (*p == '<' || *p == '>'  || *p == '='  || *p == '!' ) && *(p+1) == '=') {
        cur = NewToken(TK_RESERVED, cur, p, 2);
        p+=2;
        continue;
      }
      if((*p == '+' && *(p+1) == '+' )|| (*p == '-' && *(p+1) == '-')) {
        cur = NewToken(TK_RESERVED, cur, p, 2);
        p+=2;
        continue;
      }
      if((*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '\%' ) && *(p+1) == '=') {
        cur = NewToken(TK_RESERVED, cur, p, 2);
        p+=2;
        continue;
      }
      if(*p == '-' && *(p+1) == '>') {
        cur = NewToken(TK_RESERVED, cur, p, 2);
        p+=2;
        continue;
      } 
    }

    // one char punctuator
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%' || *p == '&'
        || *p == '(' || *p == ')'
        || *p == '[' || *p == ']'
        || *p == '{' || *p == '}'
        || *p == '<' || *p == '>' || *p == '='
        ||*p == ':' ||*p == ';' || *p == ',' || *p == '.') {
      cur = NewToken(TK_RESERVED, cur, p++, 1);
      continue;
    }

    // char
    if (p+2 && *p == '\'' && *(p+2) =='\'') {
      cur = NewToken(TK_CHAR, cur, p+1, 1);
      p+=3;
      continue;
    }

    // strings
    if (*p=='"') {
      int len = 0;
      ++p;
      while(p) {
        if (*p != '"') {
          ++len; ++p;
        } else {
          cur = NewToken(TK_STRINGS, cur, p-len, len);
          ++p; len=-1;
          break;
        }
      }
      if(len>=0)
        ErrorAt(token->str, "Strings is not closed.");
      continue;
    }

    if (isdigit(*p)) {
      cur = NewToken(TK_NUM, cur, p, 1); // token->str is only the first char of the digit?
      cur->val = strtol(p, &p, 10);
      continue;
    }

    ErrorAt(p, "Failed to tokenize");
  }

  NewToken(TK_EOF, cur, p, 1);
  return head.next;
}

