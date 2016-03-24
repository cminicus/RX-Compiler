//
//  token.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "token.h"

std::map<token_kind, string> Token::mapping = {
    {VAR, "var"},
    {LET, "let"},
    {IF, "if"},
    {ELSE, "else"},
    {WHILE, "while"},
    {PRINT, "print"},
    
    {OPEN_PAREN, "("},
    {CLOSE_PAREN, ")"},
    {OPEN_CURLY, "{"},
    {CLOSE_CURLY, "}"},
    {SEMI_COLON, ";"},
    
    {ASSIGN, "="},
    {PLUS, "+"},
    {MINUS, "-"},
    {MULTIPLY, "*"},
    {DIVIDE, "/"},
    {MODULO, "%"},
    
    {EQUALS, "=="},
    {NOT_EQUALS, "!="},
    {LESS_THAN, "<"},
    {LESS_THAN_EQUALS, "<="},
    {GREATER_THAN, ">"},
    {GREATER_THAN_EQUALS, ">="},
    
    {NEW_LINE, "\\n"},
    {END_OF_FILE, "eof"}
};

Token::Token() :
    kind(NONE), identifier(""), value(0),  line_position(-1), col_position(-1) {}

string Token::to_string() {
    if (kind == NONE) {
        return "";
    }
    
    std::ostringstream os;
    
    if (kind == IDENTIFIER) {
        os << "identifier<" << identifier << ">@(" << line_position;
        os << ":" << col_position << ")";
    } else if (kind == INTEGER) {
        os << "integer<" << value << ">@(" << line_position;
        os << ":" << col_position << ")";
    } else {
        os << Token::mapping[kind] << "@(" << line_position;
        os << ":" << col_position << ")";
    }
    
    return os.str();
}