![Banner](https://s-christy.com/status-banner-service/arithmetic-expression-parser/banner-slim.svg)

## Overview

This is a simple library I've put together for evaluating arithmetic
expressions. The expressions are supplied by a user as a string of characters,
and the integer result is placed in referenced variable.

## Usage

Here is some example usage of this library.

```c
int result = 0;
eval("1+2", &result);
printf("Answer: %d\n", result);
```

## Dependencies:

The library has no external dependencies. All you need is the C compiler that
you are using to compile the host code. To build the examples you need `gcc` and
`make`.

## Test:

`make test` will run the test program.

## Example Usage:

There is a sample program included in `./src/` called
`example_usage.c` which can be compiled and run with

```
make example
```

The sample program shows how to use the `eval()` function
to evaluate a string. Read the `Makefile` to learn how to
build it.

## License

This work is licensed under the GNU General Public License version 3 (GPLv3).

[<img src="https://s-christy.com/status-banner-service/GPLv3_Logo.svg" width="150" />](https://www.gnu.org/licenses/gpl-3.0.en.html)
