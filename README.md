## BNF
```
program        =  (func | declare_g ";" )*
func           =  type ident ("(" ( | declare ("," declare){0,5}) ")") block
block          =  "{ stmt* "}"
stmt           =  block
                  | declare_a ";"
                  | "return" expr ";"
                  | "while" "(" expr ")" stmt
                  | "for" "(" expr ";" expr ";" expr ")" stmt
                  | "if" "(" expr ")" stmt ("else" stmt)?
                  | expr ";"
type           =  ("int" | "char") "*"*
declare        =  type ident
declare_a      =  type ident ("[" num "]")?
declare_g      =  type ident ("[" num "]")?
expr           =  assign
assign         =  equality  ("=" assign)*;
equality       =  relational ( "==" | "!=" relational )*
relational     =  add ("<=" | ">=" | "<" | ">" add)*
add            =  mul ("+" | "-" mul)*
mul            =  unary ("*" | "/" | "%" unary)*
unary          =  "+" primary
                  | "-" primary
                  | "*" unary
                  | "&" unary
                  | "sizeof" unary
primary        =  num
                  | "'" char "'"
                  | ident ("(" ( | expr ("," expr){0,5}) ")")?
                  | ident "[" expr "]"
strings        =  "\"" char* "\""
```

## Known issues
* Pointer return function is not defineable.
