#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main() {
  char buf[1024];
  while (1) {
    printf("Enter an expression: ");
    int ret = scanf("%s", buf);
    if (ret == -1) {
      printf("\n");
      break;
    }
    int result = 0;
    eval(buf, &result);
    printf("Answer: %d\n\n", result);
  }
}
