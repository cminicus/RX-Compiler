//
//  code_generator.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/30/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef code_generator_h
#define code_generator_h

#include "symbol_table.h"
#include "ast.h"

class CodeGenerator {
public:
    CodeGenerator(SymbolTable & table, AST & ast) : symbol_table(table), ast(ast) {}
    void generate();

private:
    SymbolTable & symbol_table;
    AST & ast;
    
    void parse_symbol_table();
    void parse_ast();
    
    void generate_condition(ConditionNode *);
    
    void generate_instruction(InstructionNode *);
    void generate_if(IfNode *);
    void generate_while(WhileNode *);
    void generate_assign(AssignNode *);
    void generate_print(PrintNode *);
    void generate_declaration(DeclarationNode *);
    
    void generate_expression(ExpressionNode *);
    void generate_number(NumberNode *);
    void generate_binary(BinaryNode *);
    void generate_location(LocationNode *);
    void generate_variable(VariableNode *);
    void generate_constant(ConstantNode *);
};

#endif /* code_generator_h */
