//
//  error_handler.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>

#include "error_handler.h"

std::vector<std::string> ErrorHandler::error_messages;
std::vector<std::pair<int, int>> ErrorHandler::error_locations;

// --------------------------------- Errors ------------------------------------

// Scanner
std::string ErrorHandler::unterimated_comment(bool flag, int line_number, int col_number) {
    std::string s = UNTERMINATED_COMMENT(line_number, col_number);
    push_error(flag, s, line_number, col_number);
    return s;
}

std::string ErrorHandler::illegal_character(bool flag, char c, int line_number, int col_number) {
    std::string ch(1, c);
    std::string s = ILLEGAL_CHARACTER(ch, line_number, col_number);
    push_error(flag, s, line_number, col_number);
    return s;
}

// Parser
std::string ErrorHandler::unexpected_token(bool flag, Token encountered, token_kind expected) {
    
    std::string kind;
    std::string enc;
    
    if (encountered.kind == IDENTIFIER) {
        kind = "identifier";
        enc = encountered.identifier;
    } else if (encountered.kind == INTEGER) {
        kind = "integer";
        enc = std::to_string(encountered.value);
    } else {
        kind = "token";
        enc = Token::mapping[encountered.kind];
    }
    
    std::string s;
    
    if (expected == IDENTIFIER) {
        s = EXPECTED_IDENTIFIER(kind,
                                enc,
                                encountered.line_position,
                                encountered.col_position);
        
    } else if (expected == INTEGER) {
        s = EXPECTED_INTEGER(kind,
                             enc,
                             encountered.line_position,
                             encountered.col_position);
        
    } else {
        s = EXPECTED_TOKEN(kind,
                           enc,
                           Token::mapping[expected],
                           encountered.line_position,
                           encountered.col_position);
    }
 
    push_error(flag, s, encountered.line_position, encountered.col_position);
    return s;
}

std::string ErrorHandler::unexpected_token(bool flag, Token encountered, std::vector<token_kind> expected) {
    std::string kind;
    std::string enc;
    
    if (encountered.kind == IDENTIFIER) {
        kind = "identifier";
        enc = encountered.identifier;
        
    } else if (encountered.kind == INTEGER) {
        kind = "integer";
        enc = std::to_string(encountered.value);
        
    } else {
        kind = "token";
        enc = Token::mapping[encountered.kind];
    }
    
    std::string matches;
    for (int i = 0; i < (int) expected.size() - 1; i++) {
        token_kind kind = expected[i];
        if (kind == IDENTIFIER) {
            matches += "an identifier, ";
            
        } else if (kind == INTEGER) {
            matches += "an integer, ";
            
        } else {
            matches += "'" + Token::mapping[kind] + "', ";
        }
    }
    
    token_kind k = expected[expected.size() - 1];
    if (k == IDENTIFIER) {
        matches += "or an identifier";
        
    } else if (k == INTEGER) {
        matches += "or an integer";
        
    } else {
        matches += "or '" + Token::mapping[k] + "'";
    }

    std::string s = EXPECTED_MULTIPLE(kind,
                                      enc,
                                      matches,
                                      encountered.line_position,
                                      encountered.col_position);
    
    push_error(flag, s, encountered.line_position, encountered.col_position);
    return s;
}

std::string ErrorHandler::expected_instruction(bool flag, Token encountered) {
    std::string kind;
    std::string enc;
    
    if (encountered.kind == IDENTIFIER) {
        kind = "identifier";
        enc = encountered.identifier;
        
    } else if (encountered.kind == INTEGER) {
        kind = "integer";
        enc = std::to_string(encountered.value);
        
    } else {
        kind = "token";
        enc = Token::mapping[encountered.kind];
    }
    
    std::string s = EXPECTED_INSTRUCTION(kind,
                                         enc,
                                         encountered.line_position,
                                         encountered.col_position);
    
    push_error(flag, s, encountered.line_position, encountered.col_position);
    return s;
}

// Symbol Table
std::string ErrorHandler::duplicate_identifier(bool flag, Token encountered, Entry * e) {
    std::string s = DUPLICATE_IDENTIFIER(encountered.identifier,
                                         encountered.line_position,
                                         encountered.col_position,
                                         e->line_position,
                                         e->col_position);
    
    push_error(flag, s, encountered.line_position, encountered.col_position);
    return s;
}

std::string ErrorHandler::undeclared_identifier(bool flag, Token encountered) {
    std::string s = UNDECLARED_IDENTIFIER(encountered.identifier,
                                          encountered.line_position,
                                          encountered.col_position);
    
    push_error(flag, s, encountered.line_position, encountered.col_position);
    return s;
}

// AST
std::string ErrorHandler::non_variable_assignment(bool flag, Token token) {
    std::string s = NON_VARIABLE_ASSIGNMENT(token.identifier,
                                            token.line_position,
                                            token.col_position);
    
    push_error(flag, s, token.line_position, token.col_position);
    return s;
}

std::string ErrorHandler::illegal_operation_for_type(bool flag, Token token, Type * type) {
    std::string s = ILLEGAL_OPERATION_FOR_TYPE(Token::mapping[token.kind],
                                               Type::mapping[type->get_type_kind()],
                                               token.line_position,
                                               token.col_position);
    
    push_error(flag, s, token.line_position, token.col_position);
    return s;
}

std::string ErrorHandler::operation_type_mismatch(bool flag, Token token, Type * type1, Type * type2) {
    std::string s = OPERATION_TYPE_MISMATCH(Token::mapping[token.kind],
                                            Type::mapping[type1->get_type_kind()],
                                            Type::mapping[type2->get_type_kind()],
                                            token.line_position,
                                            token.col_position);
    
    push_error(flag, s, token.line_position, token.col_position);
    return s;
}

std::string ErrorHandler::incompatible_assignment(bool flag, Token token, Type * type) {
    std::string s = INCOMPATIBLE_ASSIGNMENT(token.identifier,
                                            Type::mapping[type->get_type_kind()],
                                            token.line_position,
                                            token.col_position);
    
    push_error(flag, s, token.line_position, token.col_position);
    return s;
}

std::string ErrorHandler::test_error(bool flag) {
    push_error(flag, "test", -1, -1);
    return "test";
}

// -------------------------------- Utility ------------------------------------

void ErrorHandler::push_error(bool flag, std::string error, int line, int col) {
    if (push_location(line, col)) {
        error_messages.push_back(error);
        if (flag) {
            throw_errors();
        }
    }
}

bool ErrorHandler::push_location(int line, int col) {
    std::pair<int, int> pair = {line, col};
    
    if (std::find(error_locations.begin(), error_locations.end(), pair) != error_locations.end()) {
        return false;
    }
    
    error_locations.push_back(pair);
    return true;
}

void ErrorHandler::throw_errors() {
    if (error_messages.empty()) {
        return;
    }
    
    for (auto it = error_messages.begin(); it != error_messages.end(); ++it) {
        std::cerr << "error: " << *it << std::endl;
    }
    
    int size = error_messages.size();
    error_messages.clear();
    error_locations.clear();
    
    if (size == 1) {
        throw std::runtime_error("ended with 1 error");
    } else {
        throw std::runtime_error("ended with " + std::to_string(size) + " errors");
    }
}