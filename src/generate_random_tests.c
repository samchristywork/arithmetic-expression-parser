#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Change the structure of a string full of arithmetic operators in a way that
 * preserves correctness. The character 'a' is a stand-in for a number.
 */
char *mutate(char *string) {
  char *ret = malloc(1000);
  bzero(ret, 1000);
  int idx = 0;
  for (int i = 0; i < strlen(string); i++) {
    if (string[i] == 'a') {
      int r = random();
      int n = 5;
      if (r % n == 0) {
        ret[idx] = 'a';
        ret[idx + 1] = '+';
        ret[idx + 2] = 'a';
        idx += 3;
      } else if (r % n == 1) {
        ret[idx] = 'a';
        ret[idx + 1] = '-';
        ret[idx + 2] = 'a';
        idx += 3;
      } else if (r % n == 2) {
        ret[idx] = 'a';
        ret[idx + 1] = '*';
        ret[idx + 2] = 'a';
        idx += 3;
      } else if (r % n == 3) {
        ret[idx] = 'a';
        ret[idx + 1] = '/';
        ret[idx + 2] = 'a';
        idx += 3;
      } else if (r % n == 4) {
        ret[idx] = '(';
        ret[idx + 1] = 'a';
        ret[idx + 2] = ')';
        idx += 3;
      }
    } else {
      ret[idx] = string[i];
      idx++;
    }
  }
  return ret;
}

/*
 * Generate an arithmetic string using successive calls to `mutate()`. Replaces
 * instances of the character 'a' with integers.
 */
void genString(int permutations, int range) {
  char *string;
  string = mutate("a");
  for (int i = 0; i < permutations; i++) {
    char *nextstring = mutate(string);
    free(string);
    string = nextstring;
  }
  for (int i = 0; i < strlen(string); i++) {
    if (string[i] == 'a') {
      printf("%d", (int)random() % range);
    } else {
      printf("%c", string[i]);
    }
  }
  free(string);
  printf("\n");
}

int main() {
  srand(time(0));

  /*
   * Generate 10 random strings.
   */
  for (int i = 0; i < 10; i++) {
    genString(random() % 9, random() % 1000);
  }
}
