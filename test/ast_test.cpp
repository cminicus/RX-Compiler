//
//  ast_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/12/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "ast.h"
#include "integer.h"

TEST_CASE("AST tree is properly built and destroyed") {
    
    // create condition
    ConditionNode *condition = new ConditionNode;
    condition->operation = LESS_THAN;
    
    // create number node (left side)
    NumberNode *number = new NumberNode;
    Constant *c = new Constant;
    c->type = Integer::Instance();
    c->value = 10;
    number->constant = c;
    
    // create binary node (right side)
    BinaryNode *binary = new BinaryNode;
    binary->operation = PLUS;
    
    // create left side of binary node
    NumberNode *left = new NumberNode;
    Constant *c_left = new Constant;
    c_left->type = Integer::Instance();
    c_left->value = 5;
    left->constant = c_left;
    
    // create right side of binary node
    NumberNode *right = new NumberNode;
    Constant *c_right = new Constant;
    c_right->type = Integer::Instance();
    c_right->value = 7;
    right->constant = c_right;
    
    // hook up binary node
    binary->left = left;
    binary->right = right;
    
    // hook up condition node
    condition->left = number;
    condition->right = binary;
    
    // create if node
    IfNode *if_node = new IfNode;
    if_node->condition = condition;
    
    WhileNode *while_node = new WhileNode;
    if_node->next = while_node;
    
    AssignNode *assign_node = new AssignNode;
    while_node->next = assign_node;
    
    PrintNode *print_node = new PrintNode;
    assign_node->next = print_node;
    
    // create AST set root which will be deleted automatically
    AST ast;
    ast.set_root(if_node);
    Node *root = ast.get_root();
    REQUIRE(root == if_node);
}