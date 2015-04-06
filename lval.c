#include <stdlib.h>
#include <stdio.h>
#include "lval.h"

/* Constructors. */
lval_t* lval_err(int x) {
  lval_t* lval = malloc(sizeof(lval_t));
  lval->type = LVAL_ERR;
  lval->err = x;
  return lval;
}

lval_t* lval_num(long x) {
  lval_t* lval = malloc(sizeof(lval_t));
  lval->type = LVAL_NUM;
  lval->num = x;
  return lval;
}


/* Destructors. */
void lval_destroy(lval_t* v) {
  free(v); 
}


void lval_print(lval_t* v) {
  switch (v->type) {
    case LVAL_NUM:
      _lval_print_num(v); break;
    case LVAL_ERR:
      _lval_print_err(v); break;
  }
}

void _lval_print_num(lval_t* v) {
    printf("%li", v->num);
}

void _lval_print_err(lval_t* v) {
  if (v->err == LERR_DIV_ZERO) {
    printf("Error: Division By Zero.");
  }
  if (v->err == LERR_BAD_OP) {
    printf("Error: Invalid Operator.");
  }
  if (v->err == LERR_BAD_NUM) {
    printf("Error: Invalid Number.");
  }
}

void lval_println(lval_t* v) {
  lval_print(v);
  putchar('\n');
}
