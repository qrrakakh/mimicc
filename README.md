## BNF
```
program = stmt*
stmt       = "return" expr ";"
             | { stmt* }
             | "while" "(" expr ")" stmt
             | "for" "(" expr ";" expr ";" expr ")" stmt
             | "if" "(" expr ")" stmt ("else" stmt)?
             | expr ";"
expr        = assign
assign      = equality  ("=" assign)*;
equality    = relational ( "=="|"!=" relational )*
relational  = add ("<=" || ">=" || "<" || ">" add)*
add         = mul ("+" || "-" mul)*
mul         = unary ("*" || "/" || "%" unary)*
unary       = ("+" || "-")? primary
primary     = num
              | ident ("(" ( || primary ("," primary){0,5}) ")")?
```