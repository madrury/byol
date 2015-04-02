#include "mpc/mpc.h"

long mlisp_eval(mpc_ast_t* t);
long mlisp_eval_unary_op(char* op, long x);
long mlisp_eval_binary_op(char* op, long x, long y);
