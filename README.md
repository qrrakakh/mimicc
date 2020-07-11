# mimicc: a study C compiler
This is a study work following the document below.

https://www.sigbus.info/compilerbook


## BNF
```
program         =  ((func compound_statement | declaration ";" |  "extern" declare_e "; | "extern" func ";"))*
func            =  type  "*"* ident ("(" ( | declare ("," declare){0,5}) ")")
type            =  "int" | "char" | "void" | "_Bool"
                   | struct-or-union-specifier | enum-specifier
declare         =  type "*"* ident
declaration     =  type "*"* var_a (, "*"* var_a )*
declare_e       =  type "*"* evar (, "*"* evar )*
evar            =  ident ("[" num "]")?
var_a           =  ident ("[" num "]")? ("=" assign)?

------------------------------

struct-or-union-specifier = struct-or-union identifier? "{" struct-declaration-list "}"
                            | struct-or-union identifier
sturuct-or-union = "struct"
                   | "union" ## not implemented
struct-declaration-list = struct-declaration*
struct-declaration = specifier-qualifier-list struct-declarator-list ";"
specifier-qualifier-list = type-specifier specifier-qualifier-list*
                           | type-qualifier specifier-qualifier-list*
struct-declarator-list = struct-declarator
                         | struct-declarator-list "," struct-declarator
struct-declarator = declarator

------------------------------

enum-specifier = "enum" identifier? "{" enumerator-list ","? "}"
                 | "enum" identifier
enumerator-list = enumerator
                 | enumerator-list "," enumerator
enumerator = enumeration-constant
             | enumeration-constant "=" constant-expression

------------------------------

statement = labeled-statement
            | compound-statement
            | expression-statement
            | selection-statement
            | iteration-statement
            | jump-statement

labeled-statement = identifier ":" statement  ## not implemented
                    | "case" constant-expression ":" statement
                    | "default" ":" statement

compound-statement = "{" block-item-list? "}"
block-item-list = block-item-list? block-item
block-item = declaration | statement

expression-statement = expression? ";"

selection-statement = "if" "(" expression ")" statement ("else" statement)?
                      | "switch" "(" expression ")" statement

iteration-statement = "while" "(" expression ")" statement
                      | "do" statement "while" "(" expression ")" ";" ## not implemented
                      | "for" "(" expression? ";" expression? ";" expression? ")" statement
                      | "for" "(" declaration expression? ";" expression? ")" statement

jump-statement = "goto" identifier ";" ## not implemented
                 | "continue" ";"
                 | "break" ";"
                 | "return" expression? ";"

------------------------------

expression = assignment-expression
             | expression "," assignment-expression ## not implemented

assignment-expression = conditional-expression
                        | unary-expression assignment-operator assignment-expression
assignment-operator = "=" | "*=" | "/=" | "%=" | "+=" | "-="
                      | "<<=" | ">>=" | "&=" | "^=" | "|=" ## not implemented

unary-expression = postfix-expression
                   | ++ unary-expression
                   | -- unary-expression
                   | unary-operator cast-expression
                   | "sizeof" unary-expression
                   | sizeof "(" type-name ")"  ## not implemented

unary-operator = "&" | "*" | "+" | "-"
                 | "~" | "!" ## not implemented

cast-expression = unary-expression
                   | "(" type-name ")" cast-expression ## not implemented

postfix-expression = primary-expression
                     | postfix-expression "[" expression "]"
                     | postfix-expression "(" argument-expression-list? ")"
                     | postfix-expression "." identifier
                     | postfix-expression "->" identifier
                     | postfix-expression "++"
                     | postfix-expression "--"
                     | "(" type-name ")" "{" initializer-list ","? "}" ## not implemented
argument-expression-list = (argument-expression-list "," )? assignment-expression

additive-expression = multiplicative-expression
                      | additive-expression "+" multiplicative-expression
                      | additive-expression "-" multiplicative-expression

multiplicative-expression = cast-expression
                            | multiplicative-expression "*" cast-expression
                            | multiplicative-expression "/" cast-expression
                            | multiplicative-expression "%" cast-expression

shift-expression = additive-expression
                   | shift-expression "<<" additive-expression ## not implemented
                   | shift-expression ">>" additive-expression ## not implemented

relational-expression = shift-expression
                        | relational-expression "<" shift-expression
                        | relational-expression ">" shift-expression
                        | relational-expression "<=" shift-expression
                        | relational-expression ">=" shift-expression

equality-expression = relational-expression
                      | equality-expression "==" relational-expression
                      | equality-expression "!=" relational-expression

AND-expression = equality-expression
                 | AND-expression "&" equality-expression ## not implemented
XOR-expression = AND-expression
                 | XOR-expression "^" AND-expression ## not implemented
OR-expression = XOR-expression
                | OR-expression "|" XOR-expression ## not implemented

logical-AND-expression = OR-expression
                         | logical-AND-expression "&&" XOR-expression ## not implemented
logical-OR-expression = logical-AND-expression
                        | logical-OR-expression "||" logical-AND-expression ## not implemented

conditional-expression = logical-OR-expression
                         | logical-OR-expression "?" expression ":" conditional-expression ## not implemented

constant-expression = conditional-expression
------------------------------

primary-expression = identifier
                     | constant
                     | string-literal
                     | "(" expression ")"
constant = integer-constant
           | floating-constant ## not implemented
           | enumeration-constant
           | character-constant
integer-constant = decimal-constant ## currently restricted to int integer
enumeration-constant = identifier
character-constant = ''' c-char-sequence '''
                     | 'L'' c-char-sequence ''' ## not implemented
c-char-sequence = c-char-sequence? c-char
string-literal = '"' s-char-sequence? '"'
                 | 'L'' s-char-sequence? ''' ## not implemented
s-char-sequence = s-char-sequence? c-char  ## currently resticted that c-char is equal to s-char

```

## Known issues
* External function declarement does not care its type or types of arguments.
* Struct for function argument is not suuported (struct pointer can be an argument.)
* Neverthless the function is static or or defined externally, the code calculate the address of the function dynamically when it is called.
  * Will be implemented when `static` is supported.

## BNF (To be)
### Basic structure
```
translation-unit =  external-declaration*
external-declaration = function-definition | declaration
```

### Declaration
```
function-definition = declaration-specifiers declarator declaration-list? compound-statement
declaration-list = declaration declaration*
declaration = declaration-specifiers init-declarator-list? ";"
declaration-specifiers = storage-class-specifier declaration-specifiers*
                        | type-specifier declaration-specifiers*
                        | type-qualifier declaration-specifiers*
                        | function-specifier declaration-specifiers*
storage-class-specifier = "extern"
                          | "typedef" | "static" | "auto" | "register" ## not implemented
type-specifier = "void" | "char" | "int" | "_Bool" 
                 | "short" | "long" | "float" | "double" | "signed" | "unsigned"| "_Complex" ## not implemented
                 | struct-or-union-specifier
                 | enum-specifier
                 | typedef-name ## not implemented
declarator = pointer? direct-declarator
direct-declarator = identifier
                    | "(" declarator ")"
                    | direct-declarator "(" parameter-type-list ")"
pointer = "*" type-qualifier-list? pointer?
typer-qualifier-list = type-qualifier-list? type-qualifier
parameter-type-list = parameter-list
parameter-list = (parameter-list ",")? parameter-declaration
parameter-declaration = declaration-specifiers declarator
                        | declaration-specifiers abstract-declarator?

type-name = specifier-qualifier-list abstract-declarator?
abstract-declarator = pointer | pointer? direct-abstract-declarator
direct-abstract-declarator = "(" abstract-declarator ")"
                             | direct-abstract-declarator? "(" parameter-type-list? ") "

init-declarator-list = (init-declarator-list ",")? init-declarator
init-declarator = declarator ("=" initializer)
initializer = assignment-expression
              | "{" initializer-list ","? "}"  ## not implemented
```

### Struct / Union
```
struct-or-union-specifier = struct-or-union identifier? "{" struct-declaration-list "}"
                            | struct-or-union identifier
sturuct-or-union = "struct"
                   | "union" ## not implemented
struct-declaration-list = struct-declaration*
struct-declaration = specifier-qualifier-list struct-declarator-list ";"
specifier-qualifier-list = type-specifier specifier-qualifier-list*
                           | type-qualifier specifier-qualifier-list*
struct-declarator-list = struct-declarator
                         | struct-declarator-list "," struct-declarator
struct-declarator = declarator
```
### Enum
```
enum-specifier = "enum" identifier? "{" enumerator-list ","? "}"
                 | "enum" identifier
enumerator-list = enumerator
                 | enumerator-list "," enumerator
enumerator = enumeration-constant
             | enumeration-constant "=" constant-expression
```

### Statements
```
statement = labeled-statement
            | compound-statement
            | expression-statement
            | selection-statement
            | iteration-statement
            | jump-statement

labeled-statement = identifier ":" statement  ## not implemented
                    | "case" constant-expression ":" statement
                    | "default" ":" statement

compound-statement = "{" block-item-list? "}"
block-item-list = block-item-list? block-item
block-item = declaration | statement

expression-statement = expression? ";"

selection-statement = "if" "(" expression ")" statement ("else" statement)?
                      | "switch" "(" expression ")" statement

iteration-statement = "while" "(" expression ")" statement
                      | "do" statement "while" "(" expression ")" ";" ## not implemented
                      | "for" "(" expression? ";" expression? ";" expression? ")" statement
                      | "for" "(" declaration expression? ";" expression? ")" statement

jump-statement = "goto" identifier ";" ## not implemented
                 | "continue" ";"
                 | "break" ";"
                 | "return" expression? ";"
```

### Expression
```
expression = assignment-expression
             | expression "," assignment-expression ## not implemented

assignment-expression = conditional-expression
                        | unary-expression assignment-operator assignment-expression
assignment-operator = "=" | "*=" | "/=" | "%=" | "+=" | "-="
                      | "<<=" | ">>=" | "&=" | "^=" | "|=" ## not implemented

unary-expression = postfix-expression
                   | ++ unary-expression
                   | -- unary-expression
                   | unary-operator cast-expression
                   | "sizeof" unary-expression
                   | sizeof "(" type-name ")"  ## not implemented

unary-operator = "&" | "*" | "+" | "-"
                 | "~" | "!" ## not implemented

cast-expression = unary-expression
                   | "(" type-name ")" cast-expression ## not implemented

postfix-expression = primary-expression
                     | postfix-expression "[" expression "]"
                     | postfix-expression "(" argument-expression-list? ")"
                     | postfix-expression "." identifier
                     | postfix-expression "->" identifier
                     | postfix-expression "++"
                     | postfix-expression "--"
                     | "(" type-name ")" "{" initializer-list ","? "}" ## not implemented
argument-expression-list = (argument-expression-list "," )? assignment-expression

additive-expression = multiplicative-expression
                      | additive-expression "+" multiplicative-expression
                      | additive-expression "-" multiplicative-expression

multiplicative-expression = cast-expression
                            | multiplicative-expression "*" cast-expression
                            | multiplicative-expression "/" cast-expression
                            | multiplicative-expression "%" cast-expression

shift-expression = additive-expression
                   | shift-expression "<<" additive-expression ## not implemented
                   | shift-expression ">>" additive-expression ## not implemented

relational-expression = shift-expression
                        | relational-expression "<" shift-expression
                        | relational-expression ">" shift-expression
                        | relational-expression "<=" shift-expression
                        | relational-expression ">=" shift-expression

equality-expression = relational-expression
                      | equality-expression "==" relational-expression
                      | equality-expression "!=" relational-expression

AND-expression = equality-expression
                 | AND-expression "&" equality-expression ## not implemented
XOR-expression = AND-expression
                 | XOR-expression "^" AND-expression ## not implemented
OR-expression = XOR-expression
                | OR-expression "|" XOR-expression ## not implemented

logical-AND-expression = OR-expression
                         | logical-AND-expression "&&" XOR-expression ## not implemented
logical-OR-expression = logical-AND-expression
                        | logical-OR-expression "||" logical-AND-expression ## not implemented

conditional-expression = logical-OR-expression
                         | logical-OR-expression "?" expression ":" conditional-expression ## not implemented

constant-expression = conditional-expression
```
### Primary expression / constants
```
primary-expression = identifier
                     | constant
                     | string-literal
                     | "(" expression ")"
constant = integer-constant
           | floating-constant ## not implemented
           | enumeration-constant
           | character-constant
integer-constant = decimal-constant ## currently restricted to int integer
enumeration-constant = identifier
character-constant = ''' c-char-sequence '''
                     | 'L'' c-char-sequence ''' ## not implemented
c-char-sequence = c-char-sequence? c-char
string-literal = '"' s-char-sequence? '"'
                 | 'L'' s-char-sequence? ''' ## not implemented
s-char-sequence = s-char-sequence? c-char  ## currently resticted that c-char is equal to s-char
```

### Not implemented
```
type-qualifier = "const" | "restrict" | "volatile" ## not implemented
function-specifier = "inline" ## not implemented
initializer-list = designation? initializer
                   | initializer-list "," designation? initializer
designation = designator-list "="
designator-list = designator-list? designator
designator = "[" constant-expression "]" | "." identifier
```
