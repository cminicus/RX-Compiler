//
//  parser_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "parser.h"
#include "rx_compiler.h"

void parser_correctness_test_helper(std::string program) {
    RXCompiler compiler(PARSE_ONLY);
    compiler.set_source(program);
    REQUIRE_NOTHROW(compiler.compile());
}


TEST_CASE("parser parses correctly", "[parser]") {
    
    // --------------------------- Declarations --------------------------------
    
    SECTION("simple variable declarations parse correctly") {
        parser_correctness_test_helper("var x = 4");
    }
    
    SECTION("complex variable declarations parse correctly") {
        // negative numbers have to be in parenthesis if in factor?
        // refactor negative numbers to be parse in scanner?
        parser_correctness_test_helper("var x = (4 + 3) * (-2) + (4 / 8 % 2)");
    }
    
    SECTION("simple constant declarations parse correctly") {
        parser_correctness_test_helper("let x = 4");
    }
    
    SECTION("complex constant declarations parse correctly") {
        parser_correctness_test_helper("let x = (4 + 3) * (-2) + (4 / 8 % 2)");
    }
    
    // ------------------------ Control Statements -----------------------------
    
    SECTION("empty if statement parses correctly") {
        parser_correctness_test_helper("if x == 5 {}");
    }
    
    SECTION("empty if else statement parses correctly") {
        parser_correctness_test_helper("if x == 5 {} else {}");
    }
    
    SECTION("empty if else if statement parses correctly") {
        parser_correctness_test_helper("if x == 5 {} else if 4 == 5 {}");
    }
    
    SECTION("empty if else if else statement parses correctly") {
        parser_correctness_test_helper("if x == 5 {} else if 9 == 9 {} else {}");
    }
    
    SECTION("if statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "}");
    }
    
    SECTION("if else statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "} else { \n"
                                "   x = 6; \n"
                                "}");
    }
    
    SECTION("if (else if) statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4; \n"
                                "} else if x <= 2 { \n"
                                "   x = 6 \n"
                                "}");
    }
    
    SECTION("if (else if) else statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "} else if x <= 2 { \n"
                                "   x = 6; \n"
                                "} else { \n"
                                "   x = 9; \n"
                                "}");
    }
    
    SECTION("if (else if else if) else statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "} else if x <= 2 { \n"
                                "   x = 6; \n"
                                "} else if x > 2 { \n"
                                "   x = 9 \n"
                                "} else { \n"
                                "   x = 12; \n"
                                "}");
    }
    
    SECTION("if (else if else if) else statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "} else if x <= 2 { \n"
                                "   x = 6 \n"
                                "} else if x > 2 { \n"
                                "   x = 9 \n"
                                "} else { \n"
                                "   x = 12\n"
                                "}");
    }
    
    SECTION("nested if statements parse correctly") {
        parser_correctness_test_helper("if x > 4 { \n"
                                "   let y = 4 \n"
                                "} else if x <= 2 { \n"
                                "   x = 6 \n"
                                "} else if x > 2 { \n"
                                "   x = 9 \n"
                                "   y = 8 \n"
                                "} else { \n"
                                "   x = 12 \n"
                                "   if y > 6 { \n"
                                "       x = 14 \n"
                                "   } \n"
                                "}");
    }
    
    SECTION("empty while statement parses correctly") {
        parser_correctness_test_helper("while x == 5 {}");
    }
    
    SECTION("while statements parse correctly") {
        parser_correctness_test_helper("while x > 4 { \n"
                                "   x = x + 1 \n"
                                "}");
    }
    
    SECTION("nested while statements parse correctly") {
        parser_correctness_test_helper("while x > 4 { \n"
                                "   x = x + 1 \n"
                                "   while y > 5 { \n"
                                "       y = y + 1 \n"
                                "   } \n"
                                "}");
    }

    SECTION("nested control statements parse correctly") {
        parser_correctness_test_helper("while x > 4 { \n"
                                "   x = x + 1 \n"
                                "   if y > 5 { \n"
                                "       y = y + 1 \n"
                                "       while y < 9 { \n"
                                "           y = y + 1 \n"
                                "           print(y) \n"
                                "       } \n"
                                "   } \n"
                                "}");
    }
    
    // ---------------------------- Read/Print ---------------------------------
    
    SECTION("print statements parse numbers correctly") {
        parser_correctness_test_helper("print(4)");
    }
    
    SECTION("print statements parse identifiers correctly") {
        parser_correctness_test_helper("print(y)");
    }
    
    SECTION("print statements parse number expressions correctly") {
        parser_correctness_test_helper("print(4 + 3)");
    }
    
    SECTION("print statements parse variable expressions correctly") {
        parser_correctness_test_helper("print(x + 3 - y)");
    }
    
    SECTION("scan statements parse correctly") {
        parser_correctness_test_helper("scan(x)");
    }
}

void parser_exception_test_helper(std::string program) {
    RXCompiler compiler(PARSE_ONLY);
    compiler.set_source(program);
    REQUIRE_THROWS(compiler.compile());
}

TEST_CASE("parser throws correctly for variable declarations") {
    
    // technically valid -> this is an assignment (but for an undeclared variable in this case)
//    SECTION("missing 'var'") {
//        parser_exception_test_helper("x = 4");
//    }
    
    SECTION("missing identifier") {
        parser_exception_test_helper("var = 4");
    }
    
    SECTION("missing '='") {
        parser_exception_test_helper("var x 4");
    }
    
    SECTION("missing expression") {
        parser_exception_test_helper("var x =");
    }
    
    SECTION("uses a reserved keyword") {
        parser_exception_test_helper("var while != 4");
    }
    
    SECTION("incorrect assign operator") {
        parser_exception_test_helper("var x < 4");
    }
    
    // also technically valid -> assignment from another variable
//    SECTION("not using an expression") {
//        parser_exception_test_helper("var x = hello");
//    }
}

TEST_CASE("parser throws correctly for constant declarations") {
    
    // technically valid -> this is an assignment (but for an undeclared variable in this case)
//    SECTION("missing 'let'") {
//        parser_exception_test_helper("x = 4");
//    }
    
    SECTION("missing identifier") {
        parser_exception_test_helper("let = 4");
    }
    
    SECTION("missing '='") {
        parser_exception_test_helper("let x 4");
    }
    
    SECTION("missing expression") {
        parser_exception_test_helper("let x =");
    }
    
    SECTION("uses a reserved keyword") {
        parser_exception_test_helper("let if != 4");
    }
    
    SECTION("incorrect assign operator") {
        parser_exception_test_helper("let x < 4");
    }
    
    // also technically valid -> assignment from another variable
//    SECTION("not using an expression") {
//        parser_exception_test_helper("let x = goodbye");
//    }
}

TEST_CASE("parser throws correctly for expressions") {
    
    SECTION("missing second term") {
        parser_exception_test_helper("let x = 2 +");
    }
    
    SECTION("missing second factor") {
        parser_exception_test_helper("let x = 2 + 3 *");
    }
    
    SECTION("missing second expression") {
        parser_exception_test_helper("let x = 2 + 3 * (");
    }
    
    SECTION("unclosed expression") {
        parser_exception_test_helper("let x = 2 + 3 * (2 / 3");
    }
    
    SECTION("unmatched parenthesis") {
        parser_exception_test_helper("let x = 2 + 3 * (2 / 3))");
    }
}

TEST_CASE("parser throws correctly for if statements") {
    
    SECTION("missing one side of comparison") {
        parser_exception_test_helper("if == 4 {}");
    }
    
    SECTION("missing comparison") {
        parser_exception_test_helper("if x 4 {}");
    }
    
    SECTION("missing other side of comparison") {
        parser_exception_test_helper("if x == {}");
    }
    
    SECTION("missing closing brackets") {
        parser_exception_test_helper("if x == 4 {");
    }
    
    SECTION("missing opening brackets") {
        parser_exception_test_helper("if x == 4 }");
    }
    
    SECTION("nested loop missing opening brackets") {
        parser_exception_test_helper("if x == 4 { if x == 5 }}");
    }
    
    SECTION("nested loop missing closing brackets") {
        parser_exception_test_helper("if x == 4 { if x == 5 {}");
    }
}

TEST_CASE("parser throws correctly for while statements") {

    SECTION("missing one side of comparison") {
        parser_exception_test_helper("while == 4 {}");
    }
    
    SECTION("missing comparison") {
        parser_exception_test_helper("while x 4 {}");
    }
    
    SECTION("missing other side of comparison") {
        parser_exception_test_helper("while x == {}");
    }
    
    SECTION("missing closing brackets") {
        parser_exception_test_helper("while x == 4 {");
    }
    
    SECTION("missing opening brackets") {
        parser_exception_test_helper("while x == 4 }");
    }
    
    SECTION("nested loop missing opening brackets") {
        parser_exception_test_helper("while x == 4 { while x == 5 }}");
    }
    
    SECTION("nested loop missing closing brackets") {
        parser_exception_test_helper("while x == 4 { while x == 5 {}");
    }
}

TEST_CASE("parser throws correctly for print statements") {
    
    SECTION("missing print") {
        parser_exception_test_helper("(y)");
    }
    
    SECTION("missing opening parenthesis") {
        parser_exception_test_helper("printy)");
    }
    
    SECTION("missing argument") {
        parser_exception_test_helper("print()");
    }
    
    SECTION("missing closing parenthesis") {
        parser_exception_test_helper("print(y");
    }
}

TEST_CASE("parser throws correctly for scan statements") {
    
    SECTION("missing scan") {
        parser_exception_test_helper("(y)");
    }
    
    SECTION("missing opening parenthesis") {
        parser_exception_test_helper("scany)");
    }
    
    SECTION("missing argument") {
        parser_exception_test_helper("scan()");
    }
    
    SECTION("missing closing parenthesis") {
        parser_exception_test_helper("scan(y");
    }
}
