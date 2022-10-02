![Banner](https://s-christy.com/status-banner-service/arithmetic-expression-parser/banner-slim.svg)

## Overview

This is a simple library I've put together for evaluating arithmetic
expressions. A user supplies the expressions as a string of characters, and the
integer result is placed in the referenced variable.

## Usage

Here is some example usage of this library.

```c
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main() {
  int result = 0;
  eval("1+2", &result);
  printf("Answer: %d\n", result);
}
```

One way to build this code would be the following command:

```
gcc example.c build/eval.o -I src/ -lm
```

See the Makefile for more details.

## Debug

The `eval_debug()` function is included in this library which works just like
`eval()` but prints internal status messages to `stdout`.

Here is an example of what one of these messages might look like:

```
Evaluating: "1+(2 * 3)"
Found digit: 1
Found operator: +
Found paren: (
Found digit: 2
Found operator: *
Found digit: 3
Found paren: )


Begin Token List:
PA (nil) SE 0x5560eade2810 CH 0x5560eade2850 HEAD 0
PA 0x5560eade2810 SE 0x5560eade2850 CH 0x5560eade2910 NUMBER 7
PA 0x5560eade2850 SE 0x5560eade2910 CH (nil) TAIL 0
END Token List:

Function exited successfully.
Answer: 7
```

## Limitations

This library can't presently handle floating point numbers, and will return
`EXIT_FAILURE` if one is encountered.

## Return Value

`eval` returns `EXIT_SUCCESS` if the string was successfully evaluated, and
`EXIT_FAILURE` otherwise. Please note that even in the case of failure, the
function will still make a best-effort attempt to evaluate the string which can
result in unexpected outcomes. For instance, if it is supplied the string
"2.2+1", it will drop the '.' and evaluate the string as "22+1".

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
