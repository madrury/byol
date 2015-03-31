#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

// static char input[2048];

void print_header() {
  puts("Mlisp Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");
}

int main(int argc, char** argv) {

  print_header();

  /* Event loop */
  while (1) {

    char* input = readline("mlisp> ");
    add_history(input);

    printf("You entered: %s\n", input);
    free(input);
  }

  return 0;
}
