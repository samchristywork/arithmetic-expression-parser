#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "eval.h"
#include "version.h"

int failed = 0;
int verbose = 0;

/*
 * These are the tokens that can be matched to the various lexemes read by the
 * program.
 */
typedef enum TOKEN {
  UNKNOWN = 0,
  DIGIT,
  NUMBER,
  BLANK,
  OPERATOR,
  PAREN,
  HEAD,
  TAIL,
  ERROR,
} token_t;

/*
 * This is a simple binary tree. Every node in the tree has a type and value
 * corresponding to the semantic meaning for that token.
 */
struct token {
  char type;
  int value;
  struct token *rhs;
  struct token *lhs;
};

/*
 * These globals help track memory allocation. At the end of program execution,
 * the number of mallocs should match the number of frees. Not currently used,
 * but I keep them around for debugging.
 */
int nmallocs = 0;
int nfrees = 0;

/*
 * Adds a node to the right of the node that is passed to it.
 */
struct token *add_node(struct token *n, char type, int value) {
  nmallocs++;
  (*n).rhs = (struct token *)malloc(sizeof(struct token *));
  (*n).rhs->type = type;
  (*n).rhs->value = value;
  (*n).rhs->rhs = NULL;
  (*n).rhs->lhs = n;
  return (*n).rhs;
}

/*
 * Helper function for the lexer portion of this program.
 */
long is_operator(char c) { return (long)strchr("+-*/^", c); }

/*
 * Helper function for the lexer portion of this program.
 */
long is_paren(char c) { return (long)strchr("()", c); }

/*
 * Deletes (frees) an entire linked list.
 */
void delete_list(struct token *head) {
  struct token *n = head->rhs;
  while (1) {
    struct token *next = n->rhs;
    nfrees++;
    free(n);
    n = next;
    if (n->rhs == 0) {
      free(n);
      nfrees++;
      break;
    }
  }
}

/*
 * Turns a string into a list of tokens. Basically the lexer portion of this
 * program.
 */
struct token *tokenize(const char *string) {
  struct token head = {0};
  struct token *next;
  next = add_node(&head, 0, 0);
  for (unsigned int i = 0; i < strlen(string) + 1; i++) {
    if (string[i] == 0) {
      next = add_node(next, OPERATOR, string[i]);
      continue;
    }
    if (isdigit(string[i])) {
      if (verbose) {
        printf("Found digit: %c\n", string[i]);
      }
      next = add_node(next, DIGIT, string[i] - '0');
    } else if (isblank(string[i])) {
      next = add_node(next, BLANK, string[i]);
    } else if (is_operator(string[i])) {
      if (verbose) {
        printf("Found operator: %c\n", string[i]);
      }
      next = add_node(next, OPERATOR, string[i]);
    } else if (is_paren(string[i])) {
      if (verbose) {
        printf("Found paren: %c\n", string[i]);
      }
      next = add_node(next, PAREN, string[i]);
    } else {
      printf("Ignoring character: '%c'\n", string[i]);
      failed = 1;
    }
  }

  struct token *n = &head;
  int value = 0;
  struct token *token_list_head =
      (struct token *)malloc(sizeof(struct token *));
  token_list_head->value = 0;
  token_list_head->type = HEAD;
  struct token *token_list_node = token_list_head;
  while (n->rhs != 0) {
    value *= 10;
    value += n->value;
    if (n->type != n->rhs->type ||
        n->type == PAREN) { /*
                             * TODO HACKY! But it fixes the double-parenthesis
                             * problem where if you have two parenthesis in a
                             * row the test won't pass.
                             */
      if (n->type == BLANK) {
      }
      if (n->type == OPERATOR) {
        token_list_node = add_node(token_list_node, OPERATOR, n->value);
      }
      if (n->type == PAREN) {
        token_list_node = add_node(token_list_node, PAREN, n->value);
      }
      if (n->type == DIGIT) {
        token_list_node = add_node(token_list_node, NUMBER, value);
      }
      value = 0;
    }
    n = n->rhs;
  }
  token_list_node = add_node(token_list_node, 0, 0);

  delete_list(&head);

  return token_list_head;
}

/*
 * Debug function for printing information about a token.
 */
void debug_print_token(struct token *n) {
  printf("PA %p ", (void *)n->lhs);
  printf("SE %p ", (void *)n);
  printf("CH %p ", (void *)n->rhs);
  if (n->type == OPERATOR) {
    printf("OPERATOR %c", n->value);
  } else if (n->type == PAREN) {
    printf("PAREN %c", n->value);
  } else if (n->type == HEAD) {
    printf("HEAD %d", n->value);
  } else if (n->rhs == NULL) {
    printf("TAIL %d", n->value);
  } else if (n->type == NUMBER) {
    printf("NUMBER %d", n->value);
  } else {
    printf("UNKNOWN %d", n->value);
    failed = 1;
  }
  printf("\n");
}

/*
 * debug function for printing a list of tokens.
 */
void debug_print_list(struct token *head) {
  printf("\n\nBegin Token List:\n");
  struct token *n = head;
  debug_print_token(n);
  while (n->rhs != 0) {
    n = n->rhs;
    debug_print_token(n);
  }
  printf("END Token List:\n\n");
}

/*
 * Recursively frees an entire list.
 */
void free_all(struct token *head) {
  if (head->rhs != 0) {
    free_all(head->rhs);
    free(head->rhs);
  }
}

/*
 * Removes (splices) a node from the linked list.
 */
void remove_node(struct token *n) {
  n->lhs->rhs = n->rhs;
  n->rhs->lhs = n->lhs;
  n->lhs = NULL;
  n->rhs = NULL;
  n->value = 0;
  free(n);
}

/*
 * Finds a token of a particular type and value in the linked list.
 */
struct token *find_next(struct token *head, token_t type, int value) {
  struct token *n = head;
  while (1) {
    n = n->rhs;
    if (n->type == type && n->value == value) {
      return n;
    }
  }
}

/*
 * Reverse counterpart of find_next.
 */
struct token *find_previous(struct token *head, token_t type, int value) {
  struct token *n = head;
  while (1) {
    n = n->lhs;
    if (n->type == type && n->value == value) {
      return n;
    }
    if (n->type == HEAD) {
      n->type = ERROR;
      return n;
    }
  }
}

/*
 * Debug function for printing out a list of tokens as single character values.
 */
void debug_print_expression(FILE *fileno, struct token *head, struct token *tail) {
  struct token *n;
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == NUMBER) {
      fprintf(fileno, "%d", n->value);
    }
    if (n->type == OPERATOR) {
      fprintf(fileno, "%c", n->value);
    }
    if (n->type == PAREN) {
      fprintf(fileno, "%c", n->value);
    }
  }
}

/*
 * Recursively evaluates a subset of an array of tokens. This is where most of
 * the actual parsing is done.
 */
int eval_subexpression(struct token *head, struct token *tail) {
  struct token *n;

  // Parenthesis
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == PAREN && n->value == ')') {
      struct token *p = find_previous(n, PAREN, '(');
      if (p->type == ERROR) {
        failed = 1;
        return FAILURE;
      }
      int ret = eval_subexpression(p, n);
      if (ret == FAILURE) {
        failed = 1;
        return FAILURE;
      }
      remove_node(p);
      n = n->lhs;
      remove_node(n->rhs);
    }
  }

  // Exponents
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == OPERATOR && n->value == '^') {
      int value = (int)pow(n->lhs->value, n->rhs->value);
      remove_node(n->lhs);
      remove_node(n->rhs);
      n->type = NUMBER;
      n->value = value;
    }
  }

  // Multiplication / Division
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == OPERATOR && n->value == '*') {
      int value = n->lhs->value * n->rhs->value;
      remove_node(n->lhs);
      remove_node(n->rhs);
      n->type = NUMBER;
      n->value = value;
    }
    if (n->type == OPERATOR && n->value == '/') {
      if (n->rhs->value == 0) {
        fprintf(stderr, "Error while evaluating: \"");
        debug_print_expression(stderr, head, tail);
        fprintf(stderr, "\" - Cannot divide by zero\n");
        failed = 1;
        return FAILURE;
      }
      int value = n->lhs->value / n->rhs->value;
      remove_node(n->lhs);
      remove_node(n->rhs);
      n->type = NUMBER;
      n->value = value;
    }
  }

  // Addition / Subtraction
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == OPERATOR && n->value == '+') {
      int value = n->lhs->value + n->rhs->value;
      remove_node(n->lhs);
      remove_node(n->rhs);
      n->type = NUMBER;
      n->value = value;
    }
    if (n->type == OPERATOR && n->value == '-') {
      int value = n->lhs->value - n->rhs->value;
      remove_node(n->lhs);
      remove_node(n->rhs);
      n->type = NUMBER;
      n->value = value;
    }
  }
  return SUCCESS;
}

/*
 * The main function that users will use. Pass a string in and get a result
 * assigned to the "result" variable.
 */
int eval(const char *string, int *result) {
  struct token *head = tokenize(string);

  struct token *n = head;
  while (n->rhs != 0) {
    n = n->rhs;
  }
  int ret = eval_subexpression(head, n->rhs);

  if (ret == SUCCESS && head->rhs->type != NUMBER) {
    ret = FAILURE;
  }

  if (verbose) {
    debug_print_list(head);
  }

  if (ret == SUCCESS) {
    (*result) = head->rhs->value;
  } else {
    (*result) = 0;
  }
  free_all(head);
  free(head);

  if (failed) {
    ret = FAILURE;
  }

  if (verbose) {
    if (ret == FAILURE) {
      printf("Function exited unsuccessfully.\n");
    }
    if (ret == SUCCESS) {
      printf("Function exited successfully.\n");
    }
  }

  return ret;
}

/*
 * Identical to eval, but also prints internal status messages to stdout.
 */
int eval_debug(const char *string, int *result) {
  printf("Evaluating: \"%s\"\n", string);
  verbose = 1;
  return eval(string, result);
}

void print_version() {
  printf("%s\n\n", VERSION_STRING);
  printf("%s\n", LICENSE_STRING);
}
