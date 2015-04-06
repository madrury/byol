#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

#include "parser.h"
#include "eval.h"
#include "lval.h"
#include "mpc/mpc.h"

void print_header() {
    puts("Mlisp Version 0.0.1");
    puts("Press Ctrl+c to Exit\n");
}


int main(int argc, char** argv) {

    mlisp_parser_t* parser = mlisp_parser_init();

    print_header();

    /* Event loop */
    while (1) {

        char* input = readline("mlisp> ");
        add_history(input);

        /* Attempt to parse user input. */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, parser->Lispy, &r)) {
            lval_t* result = mlisp_eval(r.output);
            lval_println(result);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    mlisp_parser_destroy(parser);
    return 0;
}
