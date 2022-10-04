#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main() {
  int result = 0;
  int return_value = eval("1+2", &result);
  if (return_value == 0) {
    printf("Answer: %d\n", result);
  } else {
    printf("Expression could not be evaluated.\n");
  }
}
