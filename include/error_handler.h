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
#include "type.h"
#include "entry.h"

class ErrorHandler {
public:
    
    // scanner
    static std::string unterimated_comment(bool, int, int);
    static std::string illegal_character(bool, char, int, int);
    
    // parser
    static std::string unexpected_token(bool, Token, token_kind);
    static std::string unexpected_token(bool, Token, std::vector<token_kind>);
    static std::string expected_instruction(bool, Token);
    
    // symbol table
    static std::string duplicate_identifier(bool, Token, Entry *);
    static std::string undeclared_identifier(bool, Token);
    
    // ast
    static std::string non_variable_assignment(bool, Token);
    static std::string illegal_operation_for_type(bool, Token, Type *);
    static std::string operation_type_mismatch(bool, Token, Type *, Type *);
    static std::string incompatible_assignment(bool, Token, Type *);
    
    static void throw_errors();
    
private:
    static std::vector<std::pair<int, int>> error_locations;
    static std::vector<std::string> error_messages;
    
    static void push_error(bool, std::string, int, int);
    static bool push_location(int, int);
};

#endif /* error_handler_h */
