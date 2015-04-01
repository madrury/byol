#include "mpc/mpc.h"

typedef struct mlisp_parser_t {
  mpc_parser_t* Number;
  mpc_parser_t* Operator;
  mpc_parser_t* Expr;
  mpc_parser_t* Lispy;
} mlisp_parser_t;

mlisp_parser_t* mlisp_parser_init(void);
void mlisp_parser_destroy(mlisp_parser_t* parser);
