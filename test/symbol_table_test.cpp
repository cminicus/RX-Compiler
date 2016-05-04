//
//  symbol_table_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "rx_compiler.h"

TEST_CASE("symbol table functions works correctly") {
    SymbolTable s;
    
    Constant *c = new Constant;
    c->value = 15;
    c->type = new Integer(32);
    
    Variable *v = new Variable;
    v->value = 20;
    v->type = new Integer(32);
    
    Constant *c2 = new Constant;
    c2->value = 25;
    c2->type = new Integer(32);
    
    SECTION("symbol table insert, find, and local works") {
        s.insert("c", c);
        s.insert("v", v);
        
        REQUIRE(s.find("c") == c);
        REQUIRE(s.find("v") == v);
        REQUIRE(s.find("c2") == nullptr);
        
        REQUIRE(s.local("c"));
        REQUIRE(s.local("v"));
        REQUIRE(!s.find("c2"));
        
        delete c2;
    }

    SECTION("symbol table new scope works") {
        s.insert("c", c);
        s.insert("v", v);
        
        s.create_scope();
        
        s.insert("c2", c2);
        
        REQUIRE(s.find("c") == c);
        REQUIRE(s.find("c2") == c2);
        REQUIRE(s.find("v") == v);
        
        REQUIRE(!s.local("c"));
        REQUIRE(!s.local("v"));
        REQUIRE(s.local("c2"));
        
        s.close_scope();
        
        REQUIRE(s.local("c"));
        REQUIRE(s.local("v"));
        REQUIRE(!s.local("c2"));
        
        REQUIRE(s.find("c2") == nullptr);
    }
}

void symbol_correctness_test_helper(std::string program) {
    RXCompiler compiler(PARSE_WITH_SYMBOLS);
    compiler.set_source(program);
    REQUIRE_NOTHROW(compiler.compile());
}

TEST_CASE("parser with symbol table parses correctly") {
    
    SECTION("declarations are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; var y = 9");
    }
    
    SECTION("assignments are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; var y = 9; x = 9; y = 4");
    }
    
    SECTION("identifier assignments are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; var y = 9; x = y; y = x");
    }
    
    SECTION("declarations in if statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; if x < 4 { let y = 4; x = 6 }");
    }
    
    SECTION("declarations in if else if else statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; if x < 4 { let y = 4 }"
                                       "else if x > 4 { let y = 4 } else { let y = 4 }");
    }
    
    SECTION("shadowed declarations in if statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; if x < 4 { let x = 4 }");
    }
    
    SECTION("scoped declarations in if statements are parsed correctly") {
        symbol_correctness_test_helper("if 2 < 4 { let x = 4 }; let x = 6");
    }
    
    SECTION("declarations in while statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; while x < 4 { let y = 4; x = 6 }");
    }
    
    SECTION("shadowed declarations in while statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; while x < 4 { let x = 7 }");
    }
    
    SECTION("scoped declarations in while statements are parsed correctly") {
        symbol_correctness_test_helper("while 2 < 4 { let x = 4 }; let x = 6");
    }
    
    
    
    SECTION("print statements are parsed correctly") {
        symbol_correctness_test_helper("let x = 4; print(x)");
    }
    
    SECTION("scan statements are parsed correctly") {
        symbol_correctness_test_helper("var x = 4; scan(x)");
    }
}

void symbol_exception_test_helper(std::string program) {
    RXCompiler compiler(PARSE_WITH_SYMBOLS);
    compiler.set_source(program);
    REQUIRE_THROWS(compiler.compile());
}

TEST_CASE("parser with symbol table throws correctly") {
    
    SECTION("undeclared declarations throw correctly") {
        symbol_exception_test_helper("x = 4");
        symbol_exception_test_helper("if 2 < 3 {x = 4}");
    }
    
    SECTION("duplicated identifiers throw correctly") {
        symbol_exception_test_helper("let x = 4; let x = 6");
    }
    
    SECTION("print statements throw correctly") {
        symbol_exception_test_helper("print(x)");
    }
    
    SECTION("scan statements throw correctly") {
        symbol_exception_test_helper("scan(x)");
    }
}