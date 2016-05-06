# RX Compiler

## Description

The compiler is written purely in C++ and the entire front end is hand-written (without using tools such as Lex/Yacc etc). The back end targets the LLVM IR for the sake of not having to write several different backends to make the language portable. As an added bonus, LLVM worries about assembly optimizations, allowing me to focus on the front-end and seeing how languages are designed and implemented.

## Current State

Currently, all stages of the compiler are completed. The compiler will be evolving as I add features listed under future plans.

### Compiler
- Scanner
- Parser
- Symbol Table
- Abstract Syntax Tree
- LLVM IR Code Generation

### Grammar

Since this project will be changing constantly, I have made links to some google docs containing descriptions of hte language. The grammar can be found [here](https://docs.google.com/document/d/1oqIxKk8QCNCo-nlTMeSKrSpOvsPqV8-2PxNliDj9Muo/edit?usp=sharing), the language semantics can be found [here](https://docs.google.com/document/d/1n4kBa3Ji_uIQjQWdtNBeYSx-AWsPXU9Hs3eer81bGlw/edit?usp=sharing), and some code snippets can be found [here](https://docs.google.com/document/d/1lDM7neo3r-abarKH66YbxOhAkQi3OqLYjAlQpDsl1No/edit?usp=sharing)

### Types

- Integer literals
- Boolean literals

### Functionality

- Mathematical and relational operators
- If statements and while loop
- Variable declarations (both constant and variable)


## Future Plans

###Proposed Timeline
- ~~Booleans/changes to grammar to make comparisons expressions~~
- ~~Basic Unary Operators~~
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

- The rest of the major Binary/Unary Operators (++ -- ^ | & etc)
- Custom binary/unary operators/switch to precedence system
- Optionals (if let, ?, ??, etc)
- Closures
- Tuples
