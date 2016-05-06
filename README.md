# RX Compiler

## Description

This project is a toy compiler for a language currently called RX. It is written purely in C++ and from the ground up without using any tools such as Lex/Yacc and others. I'm writing this compiler mostly for fun and to see how programming languages are designed and implemented.

The compiler is written purely in C++ and the entire front end is hand-written (without using tools such as Lex/Yacc etc). The back end targets the LLVM IR for the sake of not having to write several different backends to make the language portable. As an added bonus, LLVM worries about assembly optimizations, allowing me to focus on the front-end fun stuff.

## Current State

Currently I'm working on finishing the RX compiler while finishing my Compilers course. I'm implementing the compiler for the features shown below. After finishing the compiler version, I'm going to add features in the future plans section to it.

### Compiler
- Scanner
- Parser
- Symbol Table
- Abstract Syntax Tree
- LLVM IR Code Generation

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

- Scan statements

```
scan(x)
```

### Types

- Integer literals

### Functionality

- Mathematical operators
- If statements and while loop
- Variable declarations (both constant and variable)


## Future Plans

###Proposed Timeline
- **DONE:** ~~Booleans/changes to grammar to make comparisons expressions~~
- **DONE:** Basic Unary Operators
- Actual use type inference/annotation since bool is the second type
- Add for loops
- Add do-while loops
- Add match statements
- Add variable size integers
- Add floats/variable size floats
- Add functions
- Reformat grammar to only have instructions inside functions (only global variables and functions can be declared in outermost scope)
- Figure out how to not require forward declaration of functions (probably a pass through the file or a way to resolve functions not yet declared)
- Require a main method and have this be entry point
- Add includes (multi-file compilation)
- Add array literals
- Add string literals
- Add structs
- Add classes
- Add inheritance

**Not sure when...**

- The rest of the major Binary Operators (++ -- ^ | &)
- Custom binary/unary operators/switch to precedence system
- Optionals (if let, ?, ??, etc)
- Closures
- Tuples