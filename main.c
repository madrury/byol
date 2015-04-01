#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

#include "parser.h"
#include "mpc/mpc.h"

void print_header() {
  puts("Mlisp Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");
}

long eval_op(char* op, long x, long y) {
  if (strcmp(op, "+") == 0) { return x+y; }
  if (strcmp(op, "-") == 0) { return x-y; }
  if (strcmp(op, "*") == 0) { return x*y; }
  if (strcmp(op, "/") == 0) { return x/y; }
  return 0;
}

long eval(mpc_ast_t* t) {
  long x;
  /* Base case */
  if(strstr(t->tag, "number")) {
    x = atoi(t->contents);
  /* Recurse */
  } else {
    char* op = t->children[1]->contents;
    x = eval(t->children[2]);

    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
      x = eval_op(op, x, eval(t->children[i]));
      i++;
    }
  }
  return x;
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
      long result = eval(r.output);
      printf("%li\n", result);
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
