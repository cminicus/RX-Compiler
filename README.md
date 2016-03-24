# RX Compiler

## Description

This project is a toy compiler for a language currently called RX. It is written purely in C++ and from the ground up without using any tools such as Lex/Yacc and others. I'm writing this compiler mostly for fun and to see how programming languages are designed and implemented.

## Current State

Currently I'm working on finishing the RX compiler while finishing my Compilers course. I'm implementing the compiler for the features shown below. After finishing the compiler version, I'm going to add features in the future plans section to it.


### Compiler
- Scanner
- Parser
- Symbol Table
- Abstract Syntax Tree (not finished)
- Code Generation (not finished)

### Grammar

- Constant declarations 

``` 
let x = 4
```

- Variable declarations

```
var y = 4
```

- No semi-colons needed

```
let x = 4
var y = 4
```

- Variable assignments

```
var x = 4
x = 10
```

- If statements

```
if x < 4 {

} else if x > 4 {

} else {

}
```

- While statements

```
while x < 4 {
	
}
```

- Print statements

```
print(x)
```


### Types

- Integer literals

### Functionality

- Mathematical operators
- If statements and while loop
- Variable declarations (both constant and variable)


## (Vague) Future Plans

- Booleans, characters, and string literals
- Array and struct literals
- Functions
- Objects
