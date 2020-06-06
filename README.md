# mimicc: a study C compiler
This is a study work following the document below.

https://www.sigbus.info/compilerbook


## BNF
```
program        =  ((func block | declare_a ";" |  "extern" declare_e "; | "extern" func ";"))*
func           =  type  "*"* ident ("(" ( | declare ("," declare){0,5}) ")")
block          =  "{ stmt* "}"
stmt           =  block
                  | "case" expr ":"
                  | "default" ":"
                  | "continue" ";"
                  | "break" ";"
                  | declare_a ";"
                  | "return" (expr)? ";"
                  | "while" "(" expr ")" stmt
                  | "for" "(" expr | declare_a ";" expr ";" expr ")" stmt
                  | "if" "(" expr ")" stmt ("else" stmt)?
                  | expr ";"
                  | "switch" "(" expr ")" stmt
type           =  "int" | "char" | "void"
declare        =  type "*"* ident
declare_a      =  type "*"* var_a (, "*"* var_a )*
declare_e      =  type "*"* evar (, "*"* evar )*
evar           =  ident ("[" num "]")?
var_a          =  ident ("[" num "]")? ("=" assign)?
expr           =  assign
assign         =  equality  ("=" assign)*
                  | equality "+=" assign
                  | equality "-=" assign
                  | equality "*=" assign
                  | equality "/=" assign
                  | equality "%=" assign
equality       =  relational ( "==" | "!=" relational )*
relational     =  add ("<=" | ">=" | "<" | ">" add)*
add            =  mul ("+" | "-" mul)*
mul            =  unary ("*" | "/" | "%" unary)*
unary          =  postfix
                  | "+" unary
                  | "-" unary
                  | "*" unary
                  | "&" unary
                  | "sizeof" unary
                  | ("++" | "--") unary
postfix        =  primay
                  | postfix ("[" expr "]")?
                  | postfix ("(" ( | expr ("," expr){0,5}) ")")
                  | postfix "++"
                  | postfix "--"
primary        =  ident
                  | const
                  | string
const           = num
                  | "'" char "'"
string         =  "\"" char* "\""
```

## Known issues
* External function declarement does not care its type or types of arguments.