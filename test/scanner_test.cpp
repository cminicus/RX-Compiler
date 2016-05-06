//
//  scanner_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/2/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "scanner.h"

TEST_CASE("scanner scans correctly", "[scanner]") {
    Token t;
    
    // ------------------------------- Empty -----------------------------------
    SECTION("scanner scans empty file correctly") {
        Scanner s("");
        t = s.next();
        REQUIRE(t.to_string() == "eof@(1:1)");
    }
    
    // ----------------------------- Integers ----------------------------------
    SECTION("scanner scans single digit integers correctly") {
        Scanner s("1");
        t = s.next();
        REQUIRE(t.to_string() == "integer<1>@(1:1)");
    }
    
    SECTION("scanner scans multiple digit integers correctly") {
        Scanner s("12345");
        t = s.next();
        REQUIRE(t.to_string() == "integer<12345>@(1:1)");
    }
    
    SECTION("scanner scans multiple integers correctly") {
        Scanner s("1 23 456");
        t = s.next();
        REQUIRE(t.to_string() == "integer<1>@(1:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "integer<23>@(1:3)");
        
        t = s.next();
        REQUIRE(t.to_string() == "integer<456>@(1:6)");
    }
    
    // ---------------------------- Identifiers --------------------------------
    SECTION("scanner scans single identifiers correctly") {
        Scanner s("hello");
        t = s.next();
        REQUIRE(t.to_string() == "identifier<hello>@(1:1)");
    }
    
    SECTION("scanner scans multiple identifiers correctly") {
        Scanner s("hello world");
        t = s.next();
        REQUIRE(t.to_string() == "identifier<hello>@(1:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "identifier<world>@(1:7)");
    }
    
    // ----------------------------- Whitespace --------------------------------
    SECTION("scanner handles whitespace correctly") {
        Scanner s("\t \r \v \f");
        t = s.next();
        REQUIRE(t.to_string() == "eof@(1:8)");
    }
    
    SECTION("scanner handles new lines correctly") {
        Scanner s("\t \r \v \f\n  \n");
        
        // new line tokens are only generated if there is non-whitespace or
        // comments
        t = s.next();
        REQUIRE(t.to_string() == "eof@(3:1)");
    }
    
    // ------------------------------ Keywords ---------------------------------
    SECTION("scanner scans keywords correctly") {
        Scanner s("var let if else while print scan true false");
        t = s.next();
        REQUIRE(t.to_string() == "var@(1:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "let@(1:5)");
        
        t = s.next();
        REQUIRE(t.to_string() == "if@(1:9)");
        
        t = s.next();
        REQUIRE(t.to_string() == "else@(1:12)");
        
        t = s.next();
        REQUIRE(t.to_string() == "while@(1:17)");
        
        t = s.next();
        REQUIRE(t.to_string() == "print@(1:23)");
        
        t = s.next();
        REQUIRE(t.to_string() == "scan@(1:29)");
        
        t = s.next();
        REQUIRE(t.to_string() == "true@(1:34)");
        
        t = s.next();
        REQUIRE(t.to_string() == "false@(1:39)");
    }
    
    // ------------------------------ Symbols ----------------------------------
    SECTION("scanner scans symbols correctly") {
        Scanner s("() {} ; = + - * / % == != < <= > >= && || !");
        t = s.next();
        REQUIRE(t.to_string() == "(@(1:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == ")@(1:2)");
        
        t = s.next();
        REQUIRE(t.to_string() == "{@(1:4)");
        
        t = s.next();
        REQUIRE(t.to_string() == "}@(1:5)");
        
        t = s.next();
        REQUIRE(t.to_string() == ";@(1:7)");
        
        t = s.next();
        REQUIRE(t.to_string() == "=@(1:9)");
        
        t = s.next();
        REQUIRE(t.to_string() == "+@(1:11)");
        
        t = s.next();
        REQUIRE(t.to_string() == "-@(1:13)");
        
        t = s.next();
        REQUIRE(t.to_string() == "*@(1:15)");
        
        t = s.next();
        REQUIRE(t.to_string() == "/@(1:17)");
        
        t = s.next();
        REQUIRE(t.to_string() == "%@(1:19)");
        
        t = s.next();
        REQUIRE(t.to_string() == "==@(1:21)");
        
        t = s.next();
        REQUIRE(t.to_string() == "!=@(1:24)");
        
        t = s.next();
        REQUIRE(t.to_string() == "<@(1:27)");
        
        t = s.next();
        REQUIRE(t.to_string() == "<=@(1:29)");
        
        t = s.next();
        REQUIRE(t.to_string() == ">@(1:32)");
        
        t = s.next();
        REQUIRE(t.to_string() == ">=@(1:34)");
        
        t = s.next();
        REQUIRE(t.to_string() == "&&@(1:37)");
        
        t = s.next();
        REQUIRE(t.to_string() == "||@(1:40)");
        
        t = s.next();
        REQUIRE(t.to_string() == "!@(1:43)");
        
        t = s.next();
        REQUIRE(t.to_string() == "eof@(1:44)");
    }
    
    // ------------------------------ Comments ---------------------------------
    SECTION("scanner handles single line comments correctly") {
        Scanner s("// comment\n"
                  "var // comment\n"
                  "let");
        
        t = s.next();
        REQUIRE(t.to_string() == "var@(2:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "\\n@(2:15)");
        
        t = s.next();
        REQUIRE(t.to_string() == "let@(3:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "eof@(3:4)");
    }
    
    SECTION("scanner handles single block comments correctly") {
        Scanner s("/* comment\n"
                  "comment more\n"
                  "*/\n   \n"
                  "let");
        t = s.next();
        REQUIRE(t.to_string() == "let@(5:1)");
    }
    
    SECTION("scanner handles nested block comments correctly") {
        Scanner s("/* comment /* nested */ comment */var");
        t = s.next();
        REQUIRE(t.to_string() == "var@(1:35)");
    }
    
    // ----------------------------- Combination -------------------------------
    SECTION("scanner handles combinations correctly") {
        Scanner s("// this is a comment\n"
                  "let x = 4\n"
                  "x < 5\n"
                  "let y = x\n");
        
        t = s.next();
        REQUIRE(t.to_string() == "let@(2:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "identifier<x>@(2:5)");
        
        t = s.next();
        REQUIRE(t.to_string() == "=@(2:7)");
        
        t = s.next();
        REQUIRE(t.to_string() == "integer<4>@(2:9)");
        
        t = s.next();
        REQUIRE(t.to_string() == "\\n@(2:10)");
        
        t = s.next();
        REQUIRE(t.to_string() == "identifier<x>@(3:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "<@(3:3)");
        
        t = s.next();
        REQUIRE(t.to_string() == "integer<5>@(3:5)");
        
        t = s.next();
        REQUIRE(t.to_string() == "\\n@(3:6)");
        
        t = s.next();
        REQUIRE(t.to_string() == "let@(4:1)");
        
        t = s.next();
        REQUIRE(t.to_string() == "identifier<y>@(4:5)");
        
        t = s.next();
        REQUIRE(t.to_string() == "=@(4:7)");
        
        t = s.next();
        REQUIRE(t.to_string() == "identifier<x>@(4:9)");
        
        t = s.next();
        REQUIRE(t.to_string() == "\\n@(4:10)");
    }
}

TEST_CASE("scanner throws exceptions correctly", "[scanner]") {
    
    SECTION("scanner handles unterminated comments correctly") {
        Scanner s("/* comment");
        REQUIRE_THROWS(while (s.next().kind != END_OF_FILE) {});
    }
    
    SECTION("scanner handles illegal characters correctly") {
        Scanner s("he_y");
        REQUIRE_THROWS(while (s.next().kind != END_OF_FILE) {});
    }
}