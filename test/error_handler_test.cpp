//
//  error_handler_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "error_handler.h"
#include "variable.h" // symbol table?

void test_helper(int errors) {
    std::string string;
    if (errors == 1) {
        string = "ended with 1 error";
    } else {
        string = "ended with ";
        string += std::to_string(errors) + " errors";
    }
    
    try {
        ErrorHandler::throw_errors();
        REQUIRE(false);
    } catch (std::runtime_error &error) {
        std::cout << error.what() << std::endl << std::endl;
        REQUIRE(string.compare(error.what()) == 0);
    }
}

TEST_CASE("error_handler properly handles errors", "[error_handler]") {
    std::string error;
    
    // ------------------------------ Scanner ----------------------------------
    
    SECTION("unterminated comments handled properly") {
        error = ErrorHandler::unterimated_comment(false, 3, 4);
        REQUIRE(error == "unterminated comment starting at 3:4");
        test_helper(1);
    }
    
    SECTION("unterminated comments handled properly") {
        error = ErrorHandler::illegal_character(false, '!', 5, 6);
        REQUIRE(error == "illegal character '!' at 5:6");
        test_helper(1);
    }
    
    SECTION("multiple errors are handled properly") {
        ErrorHandler::unterimated_comment(false, 3, 4);
        ErrorHandler::illegal_character(false, '!', 5, 6);
        ErrorHandler::unterimated_comment(false, 10, 42);
        ErrorHandler::illegal_character(false, '=', 55, 86);
        test_helper(4);
    }
    
    // ------------------------------- Parser ----------------------------------
    
    SECTION("unexpected identifier handled properly") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "hello";
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::unexpected_token(false, t, EQUALS);
        REQUIRE(error == "encountered identifier 'hello' at 5:6 but expected token '=='");
        test_helper(1);
    }
    
    SECTION("unexpected integer handled properly") {
        Token t;
        t.kind = INTEGER;
        t.value = 9;
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::unexpected_token(false, t, NOT_EQUALS);
        REQUIRE(error == "encountered integer '9' at 5:6 but expected token '!='");
        test_helper(1);
    }

    SECTION("unexpected symbol handled properly") {
        Token t;
        t.kind = OPEN_CURLY;
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::unexpected_token(false, t, LESS_THAN_EQUALS);
        REQUIRE(error == "encountered token '{' at 5:6 but expected token '<='");
        test_helper(1);
    }
    
    SECTION("expected list handled properly") {
        Token t;
        t.kind = OPEN_CURLY;
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::unexpected_token(false, t, {LESS_THAN_EQUALS, IDENTIFIER, INTEGER});
        REQUIRE(error == "encountered token '{' at 5:6 but expected '<=', an identifier, or an integer");
        test_helper(1);
    }
    
    SECTION("expected instructions handled properly") {
        Token t;
        t.kind = OPEN_CURLY;
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::expected_instruction(false, t);
        REQUIRE(error == "encountered token '{' at 5:6 but expected an instruction");
        test_helper(1);
    }
    
    // ---------------------------- Symbol Table -------------------------------
    
    SECTION("undeclared identifier handled properly") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "x";
        t.line_position = 5;
        t.col_position = 6;
        
        error = ErrorHandler::undeclared_identifier(false, t);
        REQUIRE(error == "undeclared identifier 'x' at 5:6");
        test_helper(1);
    }
    
    SECTION("duplicate identifier handled properly") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "x";
        t.line_position = 15;
        t.col_position = 16;
        
        Variable * v = new Variable;
        v->line_position = 5;
        v->col_position = 6;
        
        error = ErrorHandler::duplicate_identifier(false, t, v);
        REQUIRE(error == "duplicate identifier 'x' at 15:16 conflicts with definition at 5:6");
        test_helper(1);
        
        delete v;
    }
    
    SECTION("identifier must denote a type") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::identifier_must_be_type(false, t);
        REQUIRE(error == "identifier 'y' at 15:16 must denote a type");
        test_helper(1);
    }
    
    // -------------------------------- AST ------------------------------------
    SECTION("non variable assignment handled properly") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::non_variable_assignment(false, t);
        REQUIRE(error == "cannot assign to non-variable identifier 'y' at 15:16");
        test_helper(1);
    }
    
    SECTION("illegal operation for type assignment handled properly") {
        Token t;
        t.kind = MINUS;
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::illegal_operation_for_type(false, t, new Boolean);
        REQUIRE(error == "cannot apply operation '-' at 15:16 with expressions of type 'bool'");
        test_helper(1);
    }
    
    SECTION("operation type mismatch assignment handled properly") {
        Token t;
        t.kind = PLUS;
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::operation_type_mismatch(false, t, new Boolean, new Boolean);
        REQUIRE(error == "cannot apply operation '+' at 15:16 with expressions of type 'bool' and 'bool'");
        test_helper(1);
    }
    
    SECTION("incompatible assignment handled properly") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::incompatible_assignment(false, t, new Boolean);
        REQUIRE(error == "cannot assign to variable 'y' at 15:16 with incompatible type 'bool'");
        test_helper(1);
    }
    
    SECTION("non-initialized variable with annotation") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::non_annotated_variable_must_have_expression(false, t);
        REQUIRE(error == "non-initialized variable 'y' at 15:16 must have a type annotation");
        test_helper(1);
    }
    
    SECTION("expression-annotation mismatch") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::expression_and_annotation_mismatch(false, t, new Boolean, new Integer);
        REQUIRE(error ==
                "cannot assign to annotated variable 'y' at 15:16 of type 'bool' with expression of type 'int'");
        test_helper(1);
    }
    
    SECTION("control expression not boolean") {
        Token t;
        t.kind = IDENTIFIER;
        t.identifier = "y";
        t.line_position = 15;
        t.col_position = 16;
        
        error = ErrorHandler::control_expression_not_boolean(false, t);
        REQUIRE(error ==
                "expression in control statement at 15:16 must be of boolean type");
        test_helper(1);
    }
    
}
