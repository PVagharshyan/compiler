/*
    program       -> stmt_list EOF

    stmt_list     -> stmt stmt_list | ε
    stmt          -> assign_stmt | if_stmt | while_stmt | for_stmt

    assign_stmt   -> IDENTIFIER "=" expr ";"
    assign_expr   -> IDENTIFIER "=" expr

    if_stmt       -> "if" "(" expr ")" block else_part
    else_part     -> "else" block | ε

    while_stmt    -> "while" "(" expr ")" block
    for_stmt      -> "for" "(" assign_expr ";" expr ";" assign_expr ")" block

    block         -> "{" stmt_list "}"

    expr          -> logical_or

    logical_or    -> logical_and logical_or'
    logical_or'   -> "||" logical_and logical_or' | ε

    logical_and   -> equality logical_and'
    logical_and'  -> "&&" equality logical_and' | ε

    equality      -> relational equality'
    equality'     -> ("==" | "!=") relational equality' | ε

    relational    -> additive relational'
    relational'   -> ("<" | "<=" | ">" | ">=") additive relational' | ε

    additive      -> term additive'
    additive'     -> ("+" | "-") term additive' | ε

    term          -> factor term'
    term'         -> ("*" | "/") factor term' | ε
    
    factor        -> IDENTIFIER | NUMBER | "(" expr ")"
*/