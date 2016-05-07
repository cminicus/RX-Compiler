//
//  token.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef token_h
#define token_h

#include <iostream>
#include <sstream>
#include <map>

enum token_kind {
    // default
    NONE = -1,
    
    // types
    IDENTIFIER,
    INTEGER,
    
    // keywords
    VAR,
    LET,
    TRUE_TOK,
    FALSE_TOK,
    IF,
    ELSE,
    WHILE,
    FOR,
    PRINT,
    SCAN,
    
    // symbols
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    SEMI_COLON,
    COLON,
    COMMA,
    
    ASSIGN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    
    EQUALS,
    NOT_EQUALS,
    LESS_THAN,
    LESS_THAN_EQUALS,
    GREATER_THAN,
    GREATER_THAN_EQUALS,
    
    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,
    
    // utility
    NEW_LINE,
    END_OF_FILE
};

class Token {
public:
    Token();
    
    token_kind kind;
    std::string identifier;
    int value;
    
    int line_position;
    int col_position;
    
    std::string to_string();
    
    static std::map<token_kind, std::string> mapping;
};
#endif /* token_h */
