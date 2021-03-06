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
    Parser(Scanner &);
    Parser(Scanner &, SymbolTable *);
    Parser(Scanner &, SymbolTable *, AST *);
    void parse();
    
private:
    // Variables
    Scanner & scanner;
    SymbolTable * symbol_table;
    AST * ast;
    Token current;
    Token last;
    bool no_symbols = false;
    bool no_ast = false;
    
    std::vector<token_kind> instructions = {
        IF, WHILE, FOR, IDENTIFIER, PRINT, SCAN, VAR, LET
    };
    
    std::vector<token_kind> statement_end = {
        NEW_LINE, SEMI_COLON
    };
    
    std::vector<token_kind> file_end = {
        NEW_LINE, SEMI_COLON, END_OF_FILE
    };
    
    std::vector<token_kind> RelOps = {
        EQUALS, NOT_EQUALS, LESS_THAN, LESS_THAN_EQUALS,
        GREATER_THAN, GREATER_THAN_EQUALS
    };

    // these are unary operators that ONLY numbers can use
    std::vector<token_kind> UnaryNumberOps = {
        MINUS // -a
    };
    
    // these are binary operators that ONLY numbers can use
    std::vector<token_kind> BinaryNumberOps = {
        LESS_THAN, LESS_THAN_EQUALS, // a < b, a <= b
        GREATER_THAN, GREATER_THAN_EQUALS, // a > b, a >= b
        PLUS, MINUS, // a + b, a - b
        MULTIPLY, DIVIDE, MODULO // a * b, a / b, a % b
    };
    
    // these are unary operators that ONLY booleans can use
    std::vector<token_kind> UnaryBooleanOps = {
         LOGICAL_NOT, // (!)
    };
    
    // these are binary operators that ONLY booleans can use
    std::vector<token_kind> BinaryBooleanOps = {
        LOGICAL_OR, // true || false
        LOGICAL_AND // true && false
    };
    
    std::vector<token_kind> BinaryOps = {
        EQUALS, NOT_EQUALS,
        LESS_THAN, LESS_THAN_EQUALS,
        GREATER_THAN, GREATER_THAN_EQUALS,
        PLUS, MINUS,
        MULTIPLY, DIVIDE, MODULO,
        LOGICAL_OR, LOGICAL_AND
    };
    
    std::vector<token_kind> UnaryOps = {
        PLUS,  // +a
        MINUS, // -a
        LOGICAL_NOT, // !a
    };
    
    std::map<std::string, int> BinaryPrecedence = {
        
        // Multiplicative
        {Token::mapping[MULTIPLY],    150},
        {Token::mapping[DIVIDE],      150},
        {Token::mapping[MODULO],      150},
        
        // Addative
        {Token::mapping[PLUS],  140},
        {Token::mapping[MINUS], 140},
        
        // Comparative
        {Token::mapping[LESS_THAN],           130},
        {Token::mapping[LESS_THAN_EQUALS],    130},
        {Token::mapping[GREATER_THAN],        130},
        {Token::mapping[GREATER_THAN_EQUALS], 130},
        {Token::mapping[EQUALS],              130},
        {Token::mapping[NOT_EQUALS],          130},
        
        // Conjunctive
        {Token::mapping[LOGICAL_AND], 120},
        
        // Disjunctive
        {Token::mapping[LOGICAL_OR],  110},
        
        // Assignment
        {Token::mapping[ASSIGN],  90},
    };
    
    // Utilities
    void next();
    token_match match(token_kind);
    token_match match(std::vector<token_kind>);
    bool optional_match(std::vector<token_kind>);
    bool check(std::vector<token_kind>);
    void sync(std::vector<token_kind>);
    int get_token_precedence(Token);
    Type * get_type_annotation();
    
    
    // Symbol Table Functions
    void create_scope();
    Scope * close_scope();
    void insert(std::string, Entry *);
    Entry * find(std::string);
    bool local(std::string);
    
    // AST Functions
    void set_ast_root(Node *);
    void check_binary_operator_typing(Token, ExpressionNode *, ExpressionNode *);
    void check_unary_operator_typing(Token, ExpressionNode *);
    
    // Entry Functions
    Variable * create_variable_entry(token_match, Type *);
    Constant * create_constant_entry(token_match, Type *);
    
    // Grammar Functions
    ExpressionNode * Statements();
    ExpressionNode * Statement();
    
    ExpressionNode * Declaration();
    ExpressionNode * VariableDeclaration();
    ExpressionNode * ConstantDeclaration();
    
    ExpressionNode * Expression();
    ExpressionNode * BinaryExpression(int, ExpressionNode *);
    ExpressionNode * UnaryExpression();
    ExpressionNode * Primary();
    
    InstructionNode * Instructions();
    InstructionNode * Instruction();
    BlockNode * Block();
    IfNode * If();
    WhileNode * While();
    void For();
    ExpressionNode * Assign(ExpressionNode *);
    PrintNode * Print();
    ScanNode * Scan();
};

#endif /* parser_h */
