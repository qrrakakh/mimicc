## BNF
```
program        =  ((func block | declare_a ";" | "extern" declare_e "; | "extern" func ";"))*
func           =  type  "*"* ident ("(" ( | declare ("," declare){0,5}) ")")
block          =  "{ stmt* "}"
stmt           =  block
                  | declare_a ";"
                  | "return" expr ";"
                  | "while" "(" expr ")" stmt
                  | "for" "(" expr | declare_a ";" expr ";" expr ")" stmt
                  | "if" "(" expr ")" stmt ("else" stmt)?
                  | expr ";"
type           =  ("int" | "char")
declare        =  type "*"* ident
declare_a      =  type "*"* var_a (, "*"* var )*
declare_e      =  type "*"* evar (, "*"* evar )*
evar           =  ident ("[" num "]")?
var_a          =  ident ("[" num "]")? ("=" assign)?
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
                  | ("++" | "--") lval
                  | lval
                  | lval ("++" | "--") 
                  | ident ("(" ( | expr ("," expr){0,5}) ")")
lval           = ident ("[" expr "]")?
strings        =  "\"" char* "\""
```

## Known issues
* External function declarement does not care its type or types of arguments.