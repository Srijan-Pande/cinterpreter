# Cinterpreter

A simple arithmetic expression evaluator written in C. This is a project which will continue to be improved upon, with new features, so as to reach a better C interpreter.

## Description

This project parses and evaluates mathematical expressions containing integers and basic operators (`+`, `-`, `*`, `/`). It handles standard operator precedence (multiplication and division are prioritized over addition and subtraction).

## Compilation to Usage

To compile and automatically run the program, use:

```bash
make
```

## Usage

Once launched, the prompt `>>>` will appear.
- Type a mathematical expression (e.g., `10 + 2 * 5`) and press **Enter** to get the result.
- Type `clear` to clear the terminal.
- Type `exit` to quit the program.

## UPDATES MADE

The testingmain.c file when compiled can handle work with floating point numbers, integrates operators such as ^ and % with operator precedence and associativity enforced as well as C23 safety features. 

NOTE : testingmain.c requires the compiler to follow the C23 standard!
