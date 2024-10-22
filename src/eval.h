#ifndef EVAL_H
#define EVAL_H

/*
 * These are the different exit statuses of the `eval` function.
 */
typedef enum EXIT_STATUS {
  SUCCESS = 0,
  FAILURE = -1
} exit_t;

/*
 * Evaluate a string
 *
 * Parameters:
 *   string: A null-terminated string of characters representing an arithmetic expression.
 *   result: A pointer to the integer result of the evaluated expression.
 *
 * Return value:
 *   EXIT_SUCCESS if the string was successfully evaluated,
 *   EXIT_FAILURE otherwise.
 *
 * Example:
 *
 *   ```
 *   int result = 0;
 *   int ret = eval("1+2", &result);
 *   if(ret) {
 *     printf("Answer: %d\n", result);
 *   } else {
 *     printf("Syntax error.\n");
 *   }
 *   ```
 */
int eval(const char *string, int *result);

/*
 * This is the same as eval, but prints internal status messages to stdout.
 *
 * Here is an example:
 */
int eval_debug(const char *string, int *result);

void print_version();

#endif
