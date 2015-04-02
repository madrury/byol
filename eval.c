#include <math.h>
#include "eval.h"
#include "mpc/mpc.h"

long mlisp_eval(mpc_ast_t* t) {
  long x;
  /* Base case */
  if (strstr(t->tag, "number")) {
    x = atoi(t->contents);
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

long mlisp_eval_unary_op(char* op, long x) {
  if(strcmp(op, "+") == 0) { return x; }
  if(strcmp(op, "-") == 0) { return -x; }
  return 0;
}

long mlisp_eval_binary_op(char* op, long x, long y) {
  if (strcmp(op, "+") == 0) { return x+y; }
  if (strcmp(op, "-") == 0) { return x-y; }
  if (strcmp(op, "*") == 0) { return x*y; }
  if (strcmp(op, "/") == 0) { return x/y; }
  if (strcmp(op, "%") == 0) { return x%y; }
  if (strcmp(op, "^") == 0) { return pow(x, y); }
  if (strcmp(op, "max") == 0) { return x > y ? x : y; }
  if (strcmp(op, "min") == 0) { return x < y ? x : y; }
  return 0;
}
