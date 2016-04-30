//
//  ast_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/12/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "rx_compiler.h"

TEST_CASE("AST tree is properly built and destroyed") {
    
    // create condition
    ConditionNode * condition = new ConditionNode;
    condition->operation = LESS_THAN;
    
    // create number node (left side)
    Constant * c = new Constant;
    c->type = Integer::Instance();
    c->value = 10;
    NumberNode * number = new NumberNode(c);
    
    // create left side of binary node
    Constant * c_left = new Constant;
    c_left->type = Integer::Instance();
    c_left->value = 5;
    NumberNode * left = new NumberNode(c_left);
    
    // create right side of binary node
    Constant * c_right = new Constant;
    c_right->type = Integer::Instance();
    c_right->value = 7;
    NumberNode * right = new NumberNode(c_right);
    
    // create binary node (right side)
    BinaryNode * binary = new BinaryNode(PLUS, left, right);
    
    // hook up condition node
    condition->left = number;
    condition->right = binary;
    
    // create if node
    IfNode * if_node = new IfNode;
    if_node->conditions.push_back(condition);
    
    WhileNode * while_node = new WhileNode;
    if_node->next = while_node;
    
    AssignNode * assign_node = new AssignNode;
    while_node->next = assign_node;
    
    PrintNode * print_node = new PrintNode;
    assign_node->next = print_node;
    
    // create AST set root which will be deleted automatically
    AST ast;
    ast.set_root(if_node);
    Node * root = ast.get_root();
    REQUIRE(root == if_node);
}

void ast_correctness_test_helper(std::string program) {
    RXCompiler compiler(PARSE_WITH_AST);
    compiler.set_source(program);
    REQUIRE_NOTHROW(compiler.compile());
}

TEST_CASE("parser with AST parses correctly") {
    /* 
     Currently the AST adds no "positive" test cases since we only have
     one type. Once there are more types, we will be able to add tests here
    */
}

void ast_exception_test_helper(std::string program) {
    RXCompiler compiler(PARSE_WITH_AST);
    compiler.set_source(program);
    REQUIRE_THROWS(compiler.compile());
}

TEST_CASE("parser with AST throws correctly") {
    
    SECTION("non variable assignment throws correctly") {
        ast_exception_test_helper("let x = 4; x = 8");
    }
    
    SECTION("illegal operation for type throws correctly") {
        
    }
    
    SECTION("operation type mismatch throws correctly") {
        
    }
    
    SECTION("incompatible assignment throws correctly") {
        
    }
}
