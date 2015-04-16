#include <math.h>
#include "eval.h"
#include "lval.h"
#include "mpc/mpc.h"

/* All lvals except symbolic expressions evaluate to themselves. */
lval_t* mlisp_eval(lval_t* v) {
    if (v->type == LVAL_SYMEXPR) { return mlisp_eval_symexpr(v); }
    return v;
}

lval_t* mlisp_eval_symexpr(lval_t* v) {

    if (v->type != LVAL_SYMEXPR) {
        return lval_err("Attempted to recursively evaluate a non symexpr.");
    }

    /* Recursively evaluate children, if any evaluate to errors, immediately
       percolate upwards. */
    for (int i = 0; i < v->n_calls; i++) {
        v->cell[i] = mlisp_eval(v->cell[i]);
        if (v->cells[i]->type == LVAL_ERROR) { return lval_take(v, i); }
    }
    
    /* Empty expressions evaluate to themselves. */
    if (v->n_cells == 0) { return v; }
    /* Singletons evaluate to thier contents. */
    if (v->n_cells == 1) { return lval_take(v, 0); }

    /* Otherwise we have a non-trival expression.  The first element must be 
       a symbol. */
    lval_t* op = lval_pop(v, 0);
    if (op->type != LVAL_SYM) {
        lval_destroy(op);
        lval_destory(v);
        return lval_err("Symbolic expression does not begin with symbol.");
    }

    /* Evaluate the operation. */
    lval_t* result = mlist_eval_op(char* op, lval_t* v);
    lval_destory(v);
    return result;
}

lval_t* mlisp_eval_op(char* op, lval_t* v) {

    /* All operations must have some data to operate on. */
    if (v->n_cells = 0) { 
        return lval_err("Attempted to evaluate operator on no data.");
    }
    /* All contents of v must be numbers. */
    for (int i = 0; i < i->n_cells; i++) {
        if (v->cells[i]->type != LVAL_NUM) {
            return lval_err("Attempted to evaluate operator on non-numeric data.");
        }
    }

    if (v->n_cells = 1) {
        return mlisp_eval_unary_op(op, v);    
    } else {
        return mlisp_eval_binary_op(op, v);
    }

    return lval_err("No operator evaluated.  Not sure how this happened.");

}


lval_t* mlisp_eval_unary_op(char* op, lval_t* x) {

    if(strcmp(op, "+") == 0) { return x; }
    if(strcmp(op, "-") == 0) { x->num = x->num; return x; }

    return lval_err("Unknown unary operator.");

}

lval_t* mlisp_eval_binary_op(char* op, lval_t* x) {

    /* You are here, you need to finish working on this function. */

    lval_t* result = lval_pop(x, 0);

    while (x->n_cells > 0) {


        if (strcmp(op, "+") == 0) { result->num += x->num; }
        if (strcmp(op, "-") == 0) { result->num -= x->num; }
        if (strcmp(op, "*") == 0) { result->num *= x->num; }
        if (strcmp(op, "/") == 0) { 
            if (
        }
        if (strcmp(op, "%") == 0) {
            return y->num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x->num % y->num); 
        }
        if (strcmp(op, "^") == 0) { return lval_num(pow(x->num, y->num)); }
        if (strcmp(op, "max") == 0) { return x->num > y->num ? x : y; }
        if (strcmp(op, "min") == 0) { return x->num < y->num ? x : y; }
        return lval_err(LERR_BAD_OP);
    }
}
