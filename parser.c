#include "parser.h"

mlisp_parser_t* mlisp_parser_init(void) {

  mlisp_parser_t* parser = malloc(sizeof(mlisp_parser_t));

  parser->Number = mpc_new("number");
  parser->Operator = mpc_new("operator");
  parser->Expr = mpc_new("expr");
  parser->Lispy = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                                  \
  number   : /-?[0-9]+(\\.[0-9]+)?/ ;                                \
  operator : '+' | '-' | '*' | '/' | '%' | '^' | \"max\" | \"min\" ; \
  expr     : <number> | '(' <operator> <expr>+ ')' ;                 \
  lispy    : /^/ <operator> <expr>+ /$/ ;                            \
  ",
  parser->Number, parser->Operator, parser->Expr, parser->Lispy);

  return parser;
}

void mlisp_parser_destroy(mlisp_parser_t* parser) {
  mpc_cleanup(4, parser->Number, parser->Operator, parser->Expr, parser->Lispy);
  free(parser);
}
