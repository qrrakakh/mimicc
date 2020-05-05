## BNF
```
program    =  func*
func       =  type ident ("(" ( | declare ("," declare){0,5}) ")") block
block      =  "{ stmt* "}"
stmt       =  block
              | declare ";"
              | "return" expr ";"
              | "while" "(" expr ")" stmt
              | "for" "(" expr ";" expr ";" expr ")" stmt
              | "if" "(" expr ")" stmt ("else" stmt)?
              | expr ";"
type        = "int"
declare     = type ident
expr        = assign
assign      = equality  ("=" assign)*;
equality    = relational ( "==" | "!=" relational )*
relational  = add ("<=" | ">=" | "<" | ">" add)*
add         = mul ("+" | "-" mul)*
mul         = unary ("*" | "/" | "%" unary)*
unary       = "+" primary
              | "-" primary
              | "*" unary
              | "&" unary
primary     = num
              | ident ("(" ( | primary ("," primary){0,5}) ")")?
```

## Known issues
* The scope of local variables are only in function, not a block.