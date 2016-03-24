//
//  error_handler.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef error_handler_h
#define error_handler_h

#include <vector>
#include <sstream>
#include <iostream>
#include "error_messages.h"
#include "token.h"
#include "entry.h"

class ErrorHandler {
public:
    
    static std::string unterimated_comment(bool, int, int);
    static std::string illegal_character(bool, char, int, int);
    static std::string unexpected_token(bool, Token, token_kind);
    static std::string unexpected_token(bool, Token, std::vector<token_kind>);
    static std::string expected_instruction(bool, Token);
    static std::string duplicate_identifier(bool, Token, Entry *);
    static std::string undeclared_identifier(bool, Token);
    
    static void push_error(bool, std::string);
    static void throw_errors();
    
private:
    static std::vector<std::string> error_messages;
};

#endif /* error_handler_h */
