#ifndef LVAL_T
#define LVAL_T
typedef struct lval_t {
     int type;
     long num;
     int err;
} lval_t;
#endif /* LVAL_T */

#ifndef LVAL_TYPE_ENUM
#define LVAL_TYPE_ENUM
enum { LVAL_NUM, LVAL_ERR };
#endif /* LVAL_TYPE_ENUM */

#ifndef LVAL_ERR_ENUM
#define LVAL_ERR_ENUM
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };
#endif /* LVAL_ERR_ENUM */

lval_t* lval_err(int x);
lval_t* lval_num(long x);
void lval_print(lval_t* v);
void _lval_print_err(lval_t* v);
void _lval_print_num(lval_t* v);
void lval_println(lval_t* v);
void lval_destroy(lval_t* v);
