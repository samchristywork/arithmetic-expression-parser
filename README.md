![Banner](https://s-christy.com/sbs/status-banner.svg?icon=arithmetic.svg&hue=0&title=Arithmetic%20Expression%20Parser&description=PEMDAS)

## Overview

This is a simple library I've put together for evaluating arithmetic
expressions. A user supplies the expressions as a string of characters, and the
integer result is placed in the referenced variable.

## Features

- Parser for arbitrary arithmetic expressions including signed and unsigned integers, operators, and nested parentheses
- Parser respects order of operations
- Comprehensive testing
- Randomly generated (deterministic) tests
- Error checking for malformed expressions and divide by zero
- Library usage example
- Support for arbitrary length expressions
- Designed to be used as a library
- Simple, heavily annotated header file
- No dependencies
- Debugging output to print out token lists

## Usage

Here is an example that shows how to use this library.

```c
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

int main() {
  int result = 0;
  int return_value = eval("1+2", &result);
  if (return_value == 0) {
    printf("Answer: %d\n", result);
  } else {
    printf("Expression could not be evaluated.\n");
  }
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
`FAILURE` if one is encountered.

## Return Value

`eval` returns `SUCCESS` (0) if the string was successfully evaluated, and
`FAILURE` (-1) otherwise. Please note that even in the case of failure, the
function will still make a best-effort attempt to evaluate the string which can
result in unexpected outcomes. For instance, if it is supplied the string
"2.2+1", it will drop the '.' and evaluate the string as "22+1".

## Dependencies:

The library has no external dependencies. All you need is the C compiler that
you are using to compile the host code. To build the examples you need `gcc` and
`make`.

## Test:

`make test` will run the test program. You should get a result that looks like
this:

```
0 - 1 + 1=2=2
0 - 1 + 1=2=2
0 - 1 - 1=0=0
0 - 5334 - 3452 * 223 - 342 / 5=-764530=-764530
0 - 5 - 3 * 2 - 3 / 5=-1=-1
0 - (1)=1=1
0 - (1 + 1) * 2=4=4
0 - (1 + 1) * (5 * 5) / 4=12=12
0 - 2 + 0 - (6) * 1 * 5 + 5 * 4 + (7 / 6 - (5))=-12=-12
0 - 2 + 0 - (6) * 1 * 5 + 5 * 4 + (7 / 6 - (5)) - 6 * 9 * 3 / 7 - 4 - 5 - 2 - 5 / 4 * 7 - (4) * 4 / 3 / 1 - 7 / (8) * (6) * (8 * 8) + 4 / 3 * (1) + 4 + 9 + 2 + 0 * (6 / 8 * 9 - 2) - 6 - 6 / 4 + 9 / 5 - 0 * 4 / 8=-48=-48
0 - 862 + 170 - (996) * 281 * 305 + 925 * 84 + (327 / 336 - (505)) - 846 * 729 * 313 / 857 - 124 - 895 - 582 - 545 / 814 * 367 - (434) * 364 / 43 / 750 - 87 / (808) * (276) * (178 * 788) + 584 / 403 * (651) + 754 + 399 + 932 + 60 * (676 / 368 * 739 - 12) - 226 - 586 / 94 + 539 / 795 - 570 * 434 / 378=-85465336=-85465336
0 - (1+1+1)^2=9=9
0 - 2^3=8=8
0 - (1 + (1))=2=2
0 - (1 + (1) + 1) + 1=4=4
0 - (160 * 21 + 135 - 43 - 51 - 101 * 100 / 15)=2728=2728
0 - 592 * 179 + 286 + 610 * (328) / 750 / 429=106254=106254
0 - (((29 / 537 + 434 * 264) * 95 - 415 * 71 / 139 + 92 + 133 - 312 + 540) / 405 / 268 / 198 + 242 * (137) / 285 - 192 + (249 - 183 / 140 / 490) * 31 + 249 / (2) * 350 / 7 + (107) / 444 - 451 - 39 + 341 - 244 + 205 + 338 / 116)=13657=13657
0 - ((404) * 154 * 243) / (248 + 786 * 690 * 128)=0=0
0 - 16 + 102 - (47) * 42 / 37 - (80) / ((151 / 55)) / 83 - 140 / 11 - 7 + 12 / 102 - 10 + 38 / 73 * 18 / 42 - 129 - (54) / (32) * (119 - 157 / 33 + 119 * 62 - 134 * (47)) - ((132)) + (140) / 64 + 75 * 28 / 106 - (112) * 108 / 24 * 94 - 117=-48892=-48892
0 - 101 / 44 + 144 / 108 / 165 / 87 * 163 / 108 * (53 * 27 / 22 + 123) / 69 * 25 / (140) - 163 + 19 * 172 + 113 - 160 + 168 / 137 * 56 - 28 / 139 + 66 * 108 + 152 * 26 * 117 * 165 + 127 - 64 * 30 / 138 - (48) * (20) * (120) - 58 - 156 - ((49)) + 80 + 0 - 20 * 105 / (42) - 85 - 124 / (34 * 17) * (((5 * 21 + 57 - 62) + 49 - 15 / 30 * 59 + (69 + 56))) + 78 * 136 / 85 * 142 + (167) / 42 * 92 * (6) + 64 / 150 - 64 + 16 / 132 - 65 * 36 - 57 * (107) + 24 + 83 + 141 / 124 - 89 - 64 - 0 * 53 / (113 * 98) * (83 * 74 * (168) / 41 / 153 - 123 - 28 + 16 + 11 * 71 * 11 * 100 - 37 * 63 + 165) / (53 - 15) / 49 + 173 * 155 * 58 - 16 + 57 - 19 - 42 * (48) * 83 * 43 * 101 - 99 * 43 / 86 - 173 - 113 - 29 * 145 + 56 + 58 + 64 - 150 + 31 / 75 / 70 * 68 * (138) / (54) + (122 * 153) + (5) + (114) / 127 + 63 - (32) * 87 - 165 + 74 * 135 + 68 / 19 - (139) + 69 - 63 / 127 + 61 + (78 - 157)=-648930118=-648930118
-1 - 1/0=0=0
Error while evaluating: "1/0" - Cannot divide by zero
-1 - (=0=0
-1 - )=0=0
-1 - *=0=0
-1 - /=0=0
-1 - +=0=0
-1 - -=0=0
-1 - */+-=0=0
DONE 29 tests passed out of 29
```

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
