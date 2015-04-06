include <math.h>
#include "eval.h"
#include "lval.h"
#include "mpc/mpc.h"

lval_t* mlisp_eval(mpc_ast_t* t) {
    lval_t* x;

    /* Base case */
    if (strstr(t->tag, "number")) {
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    } 

    /* Recurse */
    else {

        /* Unary operator: five children of node */
        if (t->children_num == 4) {
            char* op = t->children[1]->contents;
            x = mlisp_eval_unary_op(op, mlisp_eval(t->children[2]));
        } 

        /* Binary Operator: More than five children,
           reduce over array of arguments */
        else {
            char* op = t->children[1]->contents;
            x = mlisp_eval(t->children[2]);

            int i = 3;
            while (strstr(t->children[i]->tag, "expr")) {
                x = mlisp_eval_binary_op(op, x, mlisp_eval(t->children[i]));
                i++;
            }
        }
    }
    return x;
}


lval_t* mlisp_eval_unary_op(char* op, lval_t* x) {
    /* Pass through the error. */
    if (x->type == LVAL_ERR) { return x; }

    if(strcmp(op, "+") == 0) { return x; }
    if(strcmp(op, "-") == 0) { return lval_num(- x->num); }
    return lval_err(LERR_BAD_OP);
}

lval_t* mlisp_eval_binary_op(char* op, lval_t* x, lval_t* y) {
    /* Pass through an error. */
    if (x->type == LVAL_ERR) { return x; }
    if (x->type == LVAL_ERR) { return y; }

    if (strcmp(op, "+") == 0) { return lval_num(x->num + y->num); }
    if (strcmp(op, "-") == 0) { return lval_num(x->num - y->num); }
    if (strcmp(op, "*") == 0) { return lval_num(x->num * y->num); }
    if (strcmp(op, "/") == 0) { 
        return y->num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x->num / y->num); 
    }
    if (strcmp(op, "%") == 0) {
        return y->num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x->num / y->num); 
    }
    if (strcmp(op, "^") == 0) { return lval_num(pow(x->num, y->num)); }
    if (strcmp(op, "max") == 0) { return x->num > y->num ? x : y; }
    if (strcmp(op, "min") == 0) { return x->num < y->num ? x : y; }
    return lval_err(LERR_BAD_OP);
}
