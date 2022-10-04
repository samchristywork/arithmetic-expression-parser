CC := gcc
LIBS := -lm
CFLAGS := -g -Wall -Werror -Wpedantic

all: build/eval_test build/eval.o

build/eval_test: ./src/eval.c ./src/eval.h ./src/eval_test.c
	mkdir -p build/
	${CC} ${CFLAGS} ./src/eval.c ./src/eval_test.c ${LIBS} -o $@

test: build/eval_test
	./$^

example: build/example
	./$^

build/eval.o: src/eval.c src/eval.h
	mkdir -p build/
	${CC} ${CFLAGS} -c $< -o $@

build/example: src/example_usage.c build/eval.o
	${CC} ${CFLAGS} $^ ${LIBS} -o $@

clean:
	rm -rf build/

.PHONY: all clean test example
