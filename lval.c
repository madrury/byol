#include <stdlib.h>
#include <stdio.h>
#include "lval.h"
#include "mpc/mpc.h"

/* Constructors. */
lval_t* lval_err(char* m) {
    lval_t* lval = malloc(sizeof(lval_t));
    lval->type = LVAL_ERR;
    lval->err = malloc(strlen(m) + 1);
    strcpy(lval->err, m);
    return lval;
}

lval_t* lval_num(long x) {
    lval_t* lval = malloc(sizeof(lval_t));
    lval->type = LVAL_NUM;
    lval->num = x;
    return lval;
}

lval_t* lval_sym(char* s) {
    lval_t* lval = malloc(sizeof(lval_t));
    lval->type = LVAL_SYM;
    lval->sym = malloc(strlen(s) + 1);
    strcpy(lval->sym, s);
    return lval;
}

lval_t* lval_symexpr(void) {
    lval_t* lval = malloc(sizeof(lval_t));
    lval->type = LVAL_SYMEXPR;
    lval->n_cells = 0;
    lval->cells = NULL;
    return lval;
}


/* Destructors. */
void lval_destroy(lval_t* v) {
    switch (v->type) {
        case LVAL_NUM: break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
        case LVAL_SYMEXPR: _destroy_symexpr_cells(v); break;
    }
    free(v);
}

void _destroy_symexpr_cells(lval_t* v) {
    if (v->type != LVAL_SYMEXPR) {
        printf(stderr, "Attempt to free symexpr on non symepr type!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < v->n_cells; i++) {
        lval_destroy(c->cells[i]);
    }
    free(v->cells);
}


/* Create an lval from an abstract syntax tree */
lval_t* lval_from_ast(mpc_ast_t* t) {

    lval_t* x;
    
    /* Each node in the tree must be a number, symbol or symbolic expression,
       with only symbolic expressions having child nodes. */
    if(strstr(t->tag, "number")) { x = _lval_from_num(t); }
    if(strstr(t->tag, "symbol")) { x = _lval_from_sym(t); }

    /* Root or symbolic expression. */
    if(strcmp(t->tag, ">") == 0) { x = lval_symexpr(); }
    if(strstr(t->tag, "symexpr")) { x = lval_symexpr(); }
    for (int i = 0; i < t->children_num; i++) {
        if (strcmp(t->children[i]->contents, "(") == 0 ||
            strcmp(t->children[i]->contents, ")") == 0 ||
            strcmp(t->children[i]->contents, "{") == 0 ||
            strcmp(t->children[i]->contents, "}") == 0 ) {
 
            x = _lval_symexpr_append(x, lval_from_ast(t->children[i]));

        }
    }

    return x;
}

lval_t* _lval_from_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err("Invalid number.")
}

lval_t* _lval_from_sym(mpc_ast_t* t) {
    return lval_sym(t->contents);
}

lval_t* _lval_symexpr_append(lval_t* v, lval_t* x) {
    v->n_cells++
    v->cells = ralloc(v->cells, sizeof(lval_t*) * v->n_cells);
    v->cells[v->n_cells - 1] = x;
    return v;
}


/* IO */
void lval_print(lval_t* v) {
    switch (v->type) {
        case LVAL_NUM:
            _lval_print_num(v); break;
        case LVAL_ERR:
            _lval_print_err(v); break;
        case LVAL_SYM:
            _lval_print_sym(v); break;
        case LVAL_SYMEXPR:
            _lval_print_symexpr(v, '(', ')'); break;
    }
}

void _lval_print_num(lval_t* v) {
    printf("%li", v->num);
}

void _lval_print_err(lval_t* v) {
     printf("Error: %s", v->err);
}

void _lval_print_sym(lval_t* v) {
     printf("%s", v->sym);
}

void _lval_print_symexpr(lval_t* v, char open, char close) {
    putchar(open);
    for(int i = 0; i < v->n_cells; i++) {
        lval_print(v->cells[i]);
        if( i != (v->n_cells - 1)) { putchar(' '); }
    }
    putchar(close);
}

void lval_println(lval_t* v) {
    lval_print(v);
    putchar('\n');
}
