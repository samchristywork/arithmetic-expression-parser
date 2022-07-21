all: build/eval_test

build/eval_test: ./src/eval.c ./src/eval.h ./src/eval_test.c
	mkdir -p build
	gcc -g -Wall -pedantic -Werror -Wno-unused-but-set-variable ./src/eval.c ./src/eval_test.c -lm -o build/eval_test

test: build/eval_test
	./build/eval_test

example: src/example_usage.c src/eval.c src/eval.h
	mkdir -p build
	gcc -c src/eval.c -o build/eval.o
	gcc src/example_usage.c build/eval.o -lm -o build/example
	./build/example

clean:
	rm -rf build/
