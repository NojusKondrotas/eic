## Introduction

Created by Urban Dominik Müller in 1993.
Consists of 8 commands:
| Command | Description |
| :-: | :-- |
| \> | Move the pointer to the right |
| <	| Move the pointer to the left |
| \+ | Increment the memory cell at the pointer |
| \- | Decrement the memory cell at the pointer |
| .	| Output the character signified by the cell at the pointer	|
| ,	| Input a character and store it in the cell at the pointer	|
| [	| Jump past the matching ] if the cell at the pointer is 0	|
| ]	| Jump back to the matching [ if the cell at the pointer is nonzero |

Any other character is to be treated as a comment and ignored.

The original implementation specified a tape consisting of 30,000 byte-length cells on which the commands operate. Each action is performed on the cell to which the memory pointer is pointing. Upon initialization, all cells are set to zero.

The original 240-byte implementation can be seen [here](https://www.aminet.net/search?query=brainfuck).

### Brainfuck programs
Here are a few Brainfuck programs, designed to demonstrate the esolang's main features.

Truth-Machine:
```brainfuck
,>>+<<[>]>[.].
```

Addition of two memory blocks:
```brainfuck
++++++>++++<[>+<-]>
```

The infamous 'Hello World!':
```brainfuck
>++++++++[<+++++++++>-]<.
>++++[<+++++++>-]<+.
+++++++..+++.
>>++++++[<+++++++>-]<++.
------------.
>++++++[<+++++++++>-]<+.
<.+++.------.--------.
>>>++++[<++++++++>-]<+.
>>+++++[<++>-]<.
```

Fibonacci sequence (modulo-256):
```brainfuck
program does not stop execution
until it's manually terminated
+.>+.>+<<[[->>>+>+<<<<]>[->>+>+<<<]>>.<]
```

Conditional branching, compare two inputs:
```brainfuck
output < if a is smaller than b,
> if a is larger than b, and = if a equals b
,>>,<++++++++++[
    >>>>++++++<++++++<++++++<<-
]
>>>+>+<+<<<<[
    ->>[-<]<
]
>>[
    >.[[-]>]
]
>>>[.>]
```

Multiplication of two memory blocks:
```brainfuck
,>,<[->[->+>+<<]>[-<+>]<<]>>>.
```

More brainfuck (a _ton_ of brainfuck) programs can be seen [here](https://www.brainfuck.org/).