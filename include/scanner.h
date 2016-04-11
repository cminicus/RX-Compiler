//
//  scanner.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef scanner_h
#define scanner_h

#include <iostream>

#include "token.h"

class Scanner {
public:
    Scanner(std::string);
    
    Token next();
    
private:
    bool is_whitespace(char);
    bool is_new_line(char);
    bool is_letter(char);
    bool is_digit(char);
    bool is_symbol(char);
    bool is_keyword(std::string);
    
    Token handle_integer();
    Token handle_identifier();
    Token handle_keyword(Token, std::string);
    Token handle_symbol();
    void handle_single_comment();
    void handle_block_comments();
    
    std::string source;
    int source_length;
    int position;
    
    int line_number;
    int col_number;
    
    bool content_flag;
};

#endif /* scanner_h */
