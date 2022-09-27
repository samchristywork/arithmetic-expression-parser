CC := gcc
LIBS := -lm

all: build/eval_test build/eval.o

build/eval_test: ./src/eval.c ./src/eval.h ./src/eval_test.c
	mkdir -p build/
	${CC} ./src/eval.c ./src/eval_test.c ${LIBS} -o $@

test: build/eval_test
	./$^

example: build/example
	./$^

build/eval.o: src/eval.c src/eval.h
	mkdir -p build/
	${CC} -c $< -o $@

build/example: src/example_usage.c build/eval.o
	${CC} $^ ${LIBS} -o $@

clean:
	rm -rf build/

.PHONY: clean test example
