CC := gcc
LIBS := -lm

all: build/eval_test

build/eval_test: ./src/eval.c ./src/eval.h ./src/eval_test.c
	mkdir -p build
	${CC} ./src/eval.c ./src/eval_test.c ${LIBS} -o build/eval_test

test: build/eval_test
	./build/eval_test

example: src/example_usage.c src/eval.c src/eval.h
	mkdir -p build
	${CC} -c src/eval.c -o build/eval.o
	${CC} src/example_usage.c build/eval.o ${LIBS} -o build/example
	./build/example

clean:
	rm -rf build/

.PHONY: clean test example
