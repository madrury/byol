#include "parser.h"

mlisp_parser_t* mlisp_parser_init(void) {

    mlisp_parser_t* parser = malloc(sizeof(mlisp_parser_t));

    parser->Number = mpc_new("number");
    parser->Symbol = mpc_new("symbol");
    parser->SymExpr = mpc_new("symexpr");
    parser->Expr = mpc_new("expr");
    parser->Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
            "                                                                  \
            number   : /-?[0-9]+(\\.[0-9]+)?/ ;                                \
            symbol   : '+' | '-' | '*' | '/' | '%' | '^' | \"max\" | \"min\" ; \
            symexpr  : '(' <expr>* ')';                                        \
            expr     : <number> | <symbol> | <symexpr>;                        \
            lispy    : /^/ <expr>* /$/ ;                                       \
            ",
            parser->Number, parser->Symbol, parser->SymExpr, parser->Expr, 
            parser->Lispy
    );

    return parser;
}

void mlisp_parser_destroy(mlisp_parser_t* parser) {
    mpc_cleanup(5, parser->Number, parser->Symbol, parser->SymExpr, 
                parser->Expr, parser->Lispy
    );
    free(parser);
}
