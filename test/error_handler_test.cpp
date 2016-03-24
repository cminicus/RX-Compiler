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
    
    SECTION("undeclared identifier handled properly") {
        
    }
    
    SECTION("duplicate identifier handled properly") {
        
    }
}