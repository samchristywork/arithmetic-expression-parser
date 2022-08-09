#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main() {
  char buf[1024];
  while (1) {

    /*
     * Read the user's input.
     */
    printf("Enter an expression: ");
    int ret = scanf("%s", buf);

    /*
     * If EOF is encountered, then exit the program.
     */
    if (ret < 0) {
      printf("\n");
      break;
    }

    /*
     * Evaluate the user input using the "eval" function.
     */
    int result = 0;
    eval(buf, &result);

    /*
     * Print the value of the evaluated expression.
     */
    printf("Answer: %d\n\n", result);
  }
}
