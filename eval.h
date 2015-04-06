#include "mpc/mpc.h"
#include "lval.h"

lval_t* mlisp_eval(mpc_ast_t* t);
lval_t* mlisp_eval_unary_op(char* op, lval_t* x);
lval_t* mlisp_eval_binary_op(char* op, lval_t* x, lval_t* y);
