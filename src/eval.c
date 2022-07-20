#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

struct token {
  char type;
  int value;
  struct token *rhs;
  struct token *lhs;
};

int nmallocs = 0;
int nfrees = 0;
int nthese = 0;

struct token *add_node(struct token *n, char type, int value) {
  nmallocs++;
  (*n).rhs = (struct token *)malloc(sizeof(struct token *));
  (*n).rhs->type = type;
  (*n).rhs->value = value;
  (*n).rhs->rhs = NULL;
  (*n).rhs->lhs = n;
  return (*n).rhs;
}

long isoperator(char c) { return (long)strchr("+-*/^", c); }

long isparen(char c) { return (long)strchr("()", c); }

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

struct token *tokenize(const char *string) {
  struct token head = {0};
  struct token *next;
  next = add_node(&head, 0, 0);
  for (unsigned int i = 0; i < strlen(string) + 1; i++) {
    if (isdigit(string[i])) {
      // printf("d");
      next = add_node(next, DIGIT, string[i] - '0');
    } else if (isblank(string[i])) {
      // printf("b");
      next = add_node(next, BLANK, string[i]);
    } else if (isoperator(string[i])) {
      // printf("o");
      next = add_node(next, OPERATOR, string[i]);
    } else if (isparen(string[i])) {
      // printf("p");
      next = add_node(next, PAREN, string[i]);
    } else {
      printf("not_handled: '%c'\n", string[i]);
    }
  }
  // printf("\n");

  // Print string of token tokens
  // printf("Token tokens:\n");
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
        n->type == PAREN) { // TODO HACKY! But it fixes the double-parenthesis
                            // problem where if you have two parenthesis in a
                            // row the test won't pass
      if (n->type == BLANK) {
        // printf("BLANK\n");
      }
      if (n->type == OPERATOR) {
        // printf("OPERATOR %c\n", n.value);
        token_list_node = add_node(token_list_node, OPERATOR, n->value);
      }
      if (n->type == PAREN) {
        // printf("PAREN %c\n", n.value);
        token_list_node = add_node(token_list_node, PAREN, n->value);
      }
      if (n->type == DIGIT) {
        // printf("NUMBER %d\n", value);
        token_list_node = add_node(token_list_node, NUMBER, value);
      }
      value = 0;
    }
    n = n->rhs;
  }
  token_list_node = add_node(token_list_node, 0, 0);
  // printf("%c", n.type);
  // printf("\n\n");

  delete_list(&head);

  return token_list_head;
}

void print_token(struct token *n) {
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
  }
  printf("\n");
}

void print_list(struct token *head) {
  printf("\n\nBegin Token List:\n");
  struct token *n = head;
  print_token(n);
  while (n->rhs != 0) {
    n = n->rhs;
    print_token(n);
  }
  printf("END Token List:\n\n");
}

void freeall(struct token *head) {
  if (head->rhs != 0) {
    // printf("%p\n", (void *)head.rhs);
    freeall(head->rhs);
    free(head->rhs);
  }
  // free(head.rhs->rhs);
  // free(head.rhs);
}

void remove_node(struct token *n) {
  n->lhs->rhs = n->rhs;
  n->rhs->lhs = n->lhs;
  n->lhs = NULL;
  n->rhs = NULL;
  n->value = 0;
  free(n);
}

struct token *findNext(struct token *head, token_t type, int value) {
  struct token *n = head;
  while (1) {
    n = n->rhs;
    if (n->type == type && n->value == value) {
      return n;
    }
  }
}

struct token *findPrevious(struct token *head, token_t type, int value) {
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

void print_expression(FILE *fileno, struct token *head, struct token *tail) {
  struct token *n;
  n = head;
  // printf("-Evaluating: ");
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

int eval_subexpression(struct token *head, struct token *tail) {
  struct token *n;

  // print_expression(head, tail);

  // Parenthesis
  n = head;
  while (n->rhs != tail) {
    n = n->rhs;
    if (n->type == PAREN && n->value == ')') {
      struct token *p = findPrevious(n, PAREN, '(');
      if (p->type == ERROR) {
        return EXIT_FAILURE;
      }
      int ret = eval_subexpression(p, n);
      if (ret == EXIT_FAILURE) {
        return EXIT_FAILURE;
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
        print_expression(stderr, head, tail);
        fprintf(stderr, "\" - Cannot divide by zero\n");
        return EXIT_FAILURE;
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
  return EXIT_SUCCESS;
}

int eval(const char *string, int *result) {
  // printf("\n\n\n\n\n");
  // printf("Evaluating: %s\n", string);
  struct token *head = tokenize(string);

  struct token *n = head;
  while (n->rhs != 0) {
    n = n->rhs;
  }
  int ret = eval_subexpression(head, n->rhs);

  if (head->rhs->type != NUMBER) {
    ret = EXIT_FAILURE;
  }

  // print_list(head);

  if (ret == EXIT_SUCCESS) {
    (*result) = head->rhs->value;
  } else {
    (*result) = 0;
  }
  freeall(head);
  free(head);
  return ret;
}
