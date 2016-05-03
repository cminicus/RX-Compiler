//
//  token_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "token.h"

TEST_CASE("tokens initialize correctly", "[token]") {
    Token t;
    
    SECTION("empty token initializes correctly") {
        REQUIRE(t.kind == NONE);
        REQUIRE(t.identifier == "");
        REQUIRE(t.value == 0);
        REQUIRE(t.line_position == -1);
        REQUIRE(t.col_position == -1);
        REQUIRE(t.to_string() == "");
    }
    
    t.line_position = 0;
    t.col_position = 10;
    
    SECTION("identifier token prints correctly") {
        t.kind = IDENTIFIER;
        t.identifier = "test";
        REQUIRE(t.to_string() == "identifier<test>@(0:10)");
        REQUIRE(t.value == 0);
    }
    
    SECTION("integer token prints correctly") {
        t.kind = INTEGER;
        t.value = 34;
        REQUIRE(t.to_string() == "integer<34>@(0:10)");
        REQUIRE(t.identifier == "");
    }
    
    SECTION("var token prints correctly") {
        t.kind = VAR;
        REQUIRE(t.to_string() == "var@(0:10)");
        REQUIRE(t.value == 0);
        REQUIRE(t.identifier == "");
    }
    
    SECTION("let token prints correctly") {
        t.kind = LET;
        REQUIRE(t.to_string() == "let@(0:10)");
    }
    
    SECTION("if token prints correctly") {
        t.kind = IF;
        REQUIRE(t.to_string() == "if@(0:10)");
    }
    
    SECTION("else token prints correctly") {
        t.kind = ELSE;
        REQUIRE(t.to_string() == "else@(0:10)");
    }
    
    SECTION("while token prints correctly") {
        t.kind = WHILE;
        REQUIRE(t.to_string() == "while@(0:10)");
    }
    
    SECTION("print token prints correctly") {
        t.kind = PRINT;
        REQUIRE(t.to_string() == "print@(0:10)");
    }
    
    SECTION("scan token prints correctly") {
        t.kind = SCAN;
        REQUIRE(t.to_string() == "scan@(0:10)");
    }
    
    SECTION("( token prints correctly") {
        t.kind = OPEN_PAREN;
        REQUIRE(t.to_string() == "(@(0:10)");
    }
    
    SECTION(") token prints correctly") {
        t.kind = CLOSE_PAREN;
        REQUIRE(t.to_string() == ")@(0:10)");
    }
    
    SECTION("{ token prints correctly") {
        t.kind = OPEN_CURLY;
        REQUIRE(t.to_string() == "{@(0:10)");
    }
    
    SECTION("} token prints correctly") {
        t.kind = CLOSE_CURLY;
        REQUIRE(t.to_string() == "}@(0:10)");
    }
    
    SECTION("= token prints correctly") {
        t.kind = ASSIGN;
        REQUIRE(t.to_string() == "=@(0:10)");
    }
    
    SECTION("+ token prints correctly") {
        t.kind = PLUS;
        REQUIRE(t.to_string() == "+@(0:10)");
    }
    
    SECTION("- token prints correctly") {
        t.kind = MINUS;
        REQUIRE(t.to_string() == "-@(0:10)");
    }
    
    SECTION("* token prints correctly") {
        t.kind = MULTIPLY;
        REQUIRE(t.to_string() == "*@(0:10)");
    }
    
    SECTION("/ token prints correctly") {
        t.kind = DIVIDE;
        REQUIRE(t.to_string() == "/@(0:10)");
    }
    
    SECTION("% token prints correctly") {
        t.kind = MODULO;
        REQUIRE(t.to_string() == "%@(0:10)");
    }
    
    SECTION("== token prints correctly") {
        t.kind = EQUALS;
        REQUIRE(t.to_string() == "==@(0:10)");
    }
    
    SECTION("!= token prints correctly") {
        t.kind = NOT_EQUALS;
        REQUIRE(t.to_string() == "!=@(0:10)");
    }
    
    SECTION("< token prints correctly") {
        t.kind = LESS_THAN;
        REQUIRE(t.to_string() == "<@(0:10)");
    }
    
    SECTION("<= token prints correctly") {
        t.kind = LESS_THAN_EQUALS;
        REQUIRE(t.to_string() == "<=@(0:10)");
    }
    
    SECTION("> token prints correctly") {
        t.kind = GREATER_THAN;
        REQUIRE(t.to_string() == ">@(0:10)");
    }
    
    SECTION(">= token prints correctly") {
        t.kind = GREATER_THAN_EQUALS;
        REQUIRE(t.to_string() == ">=@(0:10)");
    }
    
    SECTION("\\n token prints correctly") {
        t.kind = NEW_LINE;
        REQUIRE(t.to_string() == "\\n@(0:10)");
    }
    
    SECTION("eof token prints correctly") {
        t.kind = END_OF_FILE;
        REQUIRE(t.to_string() == "eof@(0:10)");
    }
}