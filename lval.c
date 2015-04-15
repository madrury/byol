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

/* Recursively free data. */
void _destroy_symexpr_cells(lval_t* v) {
    if (v->type != LVAL_SYMEXPR) {
        printf("Attempt to free symexpr on non symepr type!", stderr);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < v->n_cells; i++) {
        lval_destroy(v->cells[i]);
    }
    free(v->cells);
}


/**
 * Create an lval from an abstract syntax tree. 
 * 
 *   Each node in the tree can be identified using the tag attribute.
 *     number : Contents is a number, has no children.
 *     symbol : Contents is a character representing an operation. Has
 *              no children.
 *     symepr : Has children, represents a symbolic expression.
 *     > : The root node of a (sub)tree.
 *     regex : Represents an input boundry.
*/
lval_t* lval_from_ast(mpc_ast_t* t) {

    lval_t* v;
    
    if(strstr(t->tag, "number")) { v = _lval_from_num(t); }
    if(strstr(t->tag, "symbol")) { v = _lval_from_sym(t); }

    if(strcmp(t->tag, ">") == 0) { v = lval_symexpr(); }
    if(strstr(t->tag, "symexpr")) { v = lval_symexpr(); }

    for (int i = 0; i < t->children_num; i++) {
        if(!_is_metachar(t->children[i])) {
            v = lval_symexpr_append(v, lval_from_ast(t->children[i]));
        }
    }
    return v;
}

/* Does this ast represent a single metachar? */
int _is_metachar(mpc_ast_t* t) {
    return (strcmp(t->contents, "(") == 0 ||
            strcmp(t->contents, ")") == 0 ||
            strcmp(t->contents, "{") == 0 ||
            strcmp(t->contents, "}") == 0 ||
            strcmp(t->tag, "regex") == 0 
    );
}

lval_t* _lval_from_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err("Invalid number.");
}

lval_t* _lval_from_sym(mpc_ast_t* t) {
    return lval_sym(t->contents);
}

/* Extend array of lvals by one and append an lval to the end. */
lval_t* lval_symexpr_append(lval_t* v, lval_t* x) {
    if(v->type != LVAL_SYMEXPR) {
        return lval_err("Expected a symexpr for appending.");
    }
    v->n_cells++;
    v->cells = realloc(v->cells, sizeof(lval_t*) * v->n_cells);
    v->cells[v->n_cells-1] = x;
    return v;
}

/* Lookup an lval by index and remove it from the array. */
lval_t* lval_symexpr_pop(lval_t* v, int i) {
    if(i < 0) { 
        return lval_err("Negative indexing an lval not allowed."); 
    }
    if(i > v->n_cells - 1) { 
        return lval_err("Index out of bounds."); 
    }
    if(v->type != LVAL_SYMEXPR) { 
        return lval_err("Expected a symexpr for get."); 
    }
    lval_t* x = v->cells[i];
    memmove(&v->cells[i], &v->cells[i+1], sizeof(lval_t*) * (v->n_cells - i - 1));
    v->n_cells--;
    v->cells = realloc(v->cells, sizeof(lval_t*) * v->n_cells);
}

lval_t* lval_symexpr_take(lval_t* v, int i) {
    lval_t* x = lval_symexpr_take(v, i);
    lval_destroy(v);
    return x;
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

/* Print boundry parenthesis, then data. */
void _lval_print_symexpr(lval_t* v, char open, char close) {
    putchar(open);
    for(int i = 0; i < v->n_cells; i++) {
        if(v->cells[i]) {
            lval_print(v->cells[i]);
        }
        else { 
        }
        if( i != (v->n_cells - 1)) { putchar(' '); }
    }
    putchar(close);
}

void lval_println(lval_t* v) {
    lval_print(v);
    putchar('\n');
}
