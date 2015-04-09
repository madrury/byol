#include "mpc/mpc.c"

#ifndef LVAL_T
#define LVAL_T
typedef struct lval_t {
     int type;
     /* Basic types. */
     long num;
     char* err;
     char* sym;
     /* Compund type: Array of lval_t for a symexpr. */
     int n_cells;
     struct lval_t** cells;

} lval_t;
#endif /* LVAL_T */

#ifndef LVAL_TYPE_ENUM
#define LVAL_TYPE_ENUM
enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SYMEXPR };
#endif /* LVAL_TYPE_ENUM */

#ifndef LVAL_ERR_ENUM
#define LVAL_ERR_ENUM
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };
#endif /* LVAL_ERR_ENUM */

lval_t* lval_err(char* m);
lval_t* lval_num(long x);
lval_t* lval_sym(char* s);
lval_t* lval_symexpr(void);

void lval_destroy(lval_t* v);
void _destroy_symexpr_cells(lval_t* v);

lval_t* lval_from_ast(mpc_ast_t* t);
lval_t* _lval_from_num(mpc_ast_t* t);
lval_t* _lval_from_sym(mpc_ast_t* t);
lval_t* _lval_symexpr_add(lval_t* v, lval_t* x);

void lval_print(lval_t* v);
void lval_println(lval_t* v);
void _lval_print_err(lval_t* v);
void _lval_print_num(lval_t* v);
void _lval_print_sym(lval_t* v);
void _lval_print_symexpr(lval_t* v);
