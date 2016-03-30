//
//  parser.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include "scanner.h"
#include "symbol_table.h"
#include "ast.h"
#include <vector>

typedef struct dummy_token_match {
    Token token;
    bool is_valid;
} token_match;

class Parser {
public:
    Parser(Scanner&);
    Parser(Scanner&, SymbolTable*);
    Parser(Scanner&, SymbolTable*, AST*);
    void parse();
    
private:
    // Variables
    Scanner& scanner;
    SymbolTable *symbol_table;
    AST *ast;
    Token current;
    Token last;
    bool no_symbols = false;
    bool no_ast = false;
    
    std::vector<token_kind> instructions = {
        IF, WHILE, IDENTIFIER, PRINT, VAR, LET
    };
    
    std::vector<token_kind> conditions = {
        EQUALS, NOT_EQUALS, LESS_THAN, LESS_THAN_EQUALS,
        GREATER_THAN, GREATER_THAN_EQUALS
    };
    
    // Utilities
    void next();
    token_match match(token_kind);
    token_match match(std::vector<token_kind>);
    bool optional_match(std::vector<token_kind>);
    bool check(std::vector<token_kind>);
    void sync(std::vector<token_kind>);
    
    // Symbol Table Functions
    void create_scope();
    void close_scope();
    void insert(std::string, Entry *);
    Entry * find(std::string);
    bool local(std::string);
    
    // AST Functions
    void set_ast_root(Node *);
    
    // Entry Functions
    Variable * create_variable_entry(token_match);
    Constant * create_constant_entry(token_match);
    
    // Grammar Functions
    void Statements();
    
    DeclarationNode * Declaration();
    DeclarationNode * VariableDeclaration();
    DeclarationNode * ConstantDeclaration();
    
    ExpressionNode * Expression();
    ExpressionNode * Term();
    ExpressionNode * Factor();
    
    InstructionNode * Instructions();
    InstructionNode * Instruction();
    IfNode * If();
    WhileNode * While();
    AssignNode * Assign();
    PrintNode * Print();
    ConditionNode * Condition();
};

#endif /* parser_h */
