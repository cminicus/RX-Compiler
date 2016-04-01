//
//  code_generator.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/30/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "code_generator.h"

void CodeGenerator::generate() {
    parse_symbol_table();
    parse_ast();
}

void CodeGenerator::parse_symbol_table() {
    
}

void CodeGenerator::parse_ast() {
    Node * root = ast.get_root();
    generate_instruction(dynamic_cast<InstructionNode *>(root));
}

// ------------------------------- Instructions --------------------------------

void CodeGenerator::generate_instruction(InstructionNode * node) {
    if (node == nullptr) {
        return;
    }
    
    switch (node->get_instruction_kind()) {
        case IF_INSTRUCTION:          generate_if         (dynamic_cast<IfNode *>(node)); break;
        case WHILE_INSTRUCTION:       generate_while      (dynamic_cast<WhileNode *>(node)); break;
        case ASSIGN_INSTRUCTION:      generate_assign     (dynamic_cast<AssignNode *>(node)); break;
        case PRINT_INSTRUCTION:       generate_print      (dynamic_cast<PrintNode *>(node)); break;
        case DECLARATION_INSTRUCTION: generate_declaration(dynamic_cast<DeclarationNode *>(node)); break;
    }
    
    generate_instruction(node->next);
}

// should this selectively generate or always generate and let assembly figure it out?
// I don't think we can actually generate like that unless it folds away at compile time
// this is an optimization
// just generate all conditions and instructions
void CodeGenerator::generate_if(IfNode * node) {
    int length = node->conditions.size();
    
    for (int i = 0; i < length; i++) {
        ConditionNode * condition = node->conditions.at(i);
        generate_condition(condition);

        InstructionNode * instructions = node->instructions.at(i);
        generate_instruction(instructions);
    }
    
    if (node->instructions.size() > node->conditions.size()) {
        InstructionNode * else_node = node->instructions.at(node->instructions.size() - 1);
        generate_instruction(else_node);
    }
}

void CodeGenerator::generate_while(WhileNode * node) {
    generate_condition(node->condition);
    generate_instruction(node->instructions);
}

void CodeGenerator::generate_assign(AssignNode * node) {
    generate_location(node->location);
    generate_expression(node->expression);
}

void CodeGenerator::generate_print(PrintNode * node) {
    generate_location(node->location);
}

void CodeGenerator::generate_declaration(DeclarationNode * node) {
    generate_location(node->location);
    generate_expression(node->expression);
}

// -------------------------------- Conditions ---------------------------------

void CodeGenerator::generate_condition(ConditionNode * node) {
    
}

// -------------------------------- Expresions ---------------------------------

void CodeGenerator::generate_expression(ExpressionNode * node) {
    switch (node->get_expression_kind()) {
        case NUMBER_EXPRESSION:   generate_number  (dynamic_cast<NumberNode *>(node)); break;
        case BINARY_EXPRESSION:   generate_binary  (dynamic_cast<BinaryNode *>(node)); break;
        case LOCATION_EXPRESSION: generate_location(dynamic_cast<LocationNode *>(node)); break;
    }
}

void CodeGenerator::generate_number(NumberNode * node) {
    
}

void CodeGenerator::generate_binary(BinaryNode * node) {
    
}

void CodeGenerator::generate_location(LocationNode * node) {
    switch (node->get_location_kind()) {
        case VARIABLE_LOCATION: generate_variable(dynamic_cast<VariableNode *>(node)); break;
        case CONSTANT_LOCATION: generate_constant(dynamic_cast<ConstantNode *>(node)); break;
    }
}

void CodeGenerator::generate_variable(VariableNode * node) {
    
}

void CodeGenerator::generate_constant(ConstantNode * node) {
    
}
