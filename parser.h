#include "mpc/mpc.h"

#ifndef MLISP_PARSER_T
#define MLISP_PARSER_T
typedef struct mlisp_parser_t {
    mpc_parser_t* Number;
    mpc_parser_t* Operator;
    mpc_parser_t* Expr;
    mpc_parser_t* Lispy;
} mlisp_parser_t;
#endif /* MLISP_PARSER_T */

mlisp_parser_t* mlisp_parser_init(void);
void mlisp_parser_destroy(mlisp_parser_t* parser);
