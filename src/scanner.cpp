//
//  scanner.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "scanner.h"
#include "token.h"
#include "error_handler.h"

/**
 *  Creates a scanner object with the given source string
 */
Scanner::Scanner(std::string file) :
    source(file), source_length(file.length()), position(0),
    line_number(1), col_number(1), content_flag(false) {
    
}

/**
 *  Parses the source string for the next valid token
 *
 *  @return the next valid token in the source string
 */
Token Scanner::next() {
    Token t;
    
    if (position < source_length) {
        
        // bypass whitespace
        while (is_whitespace(source[position])) {
            position++;
            col_number++;
            if (position == source_length) {
                t.kind = END_OF_FILE;
                t.line_position = line_number;
                t.col_position = col_number;
                return t;
            }
        }
        
        // create digit
        if (is_digit(source[position])) {
            content_flag = true;
            return handle_integer();
            
        // create identifier
        } else if (is_letter(source[position])) {
            content_flag = true;
            return handle_identifier();
            
        // create symbols
        } else if (is_symbol(source[position])) {
            // single line comments
            if (position < source_length - 1 && source[position] == '/' &&
                (source[position + 1] == '/')) {
                
                // move past comment
                position += 2;
                col_number += 2;
                
                handle_single_comment();
                
                // handle comments and stop before newline so it's handled
                // out here
                return next();
                
            // block comments
            } else if (position < source_length - 1 && source[position] == '/' &&
                       (source[position + 1] == '*')) {
                
                // move past comment
                position += 2;
                col_number += 2;
                
                // handle comments
                handle_block_comments();
                
                // return next token after comments
                return next();
                
            } else {
                // don't set content flag for new_line
                if (source[position] != '\n') {
                    content_flag = true;
                }
                return handle_symbol();
            }
        } else {
            ErrorHandler::illegal_character(true, source[position], line_number, col_number);
        }
    }
    
    // return EOF if we've finished the source
    t.kind = END_OF_FILE;
    t.line_position = line_number;
    t.col_position = col_number;
    return t;
}

// --------------------------- Private Methods ---------------------------------

/**
 *  Determines if the given character is whitespace
 *
 *  @param c the character to check
 *
 *  @return true if the character is whitespace, false otherwise
 */
bool Scanner::is_whitespace(char c) {
    // TODO: have an is_new_line function that alters line_number and col_number
    return c == ' ' || c == '\t' || c == '\r' || c == '\v' ||
    c == '\f';
}

bool Scanner::is_new_line(char c) {
    return c == '\n';
}

/**
 *  Determines if the given character is a letter
 *
 *  @param c the character to check
 *
 *  @return true if the character is a letter, false otherwise
 */
bool Scanner::is_letter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

/**
 *  Determines if the given character is a digit
 *
 *  @param c the character to check
 *
 *  @return true if the character is a digit, false otherwise
 */
bool Scanner::is_digit(char c) {
    return ('0' <= c) && (c <= '9');
}

/**
 *  Determines if the given character is a symbol, or the beginning of a symbol
 *
 *  @param c the character to check
 *
 *  @return true if the character is a symbol, false otherwise
 */
bool Scanner::is_symbol(char c) {
    return
    c == '(' || c == ')' || c == '{' ||
    c == '}' || c == ';' || c == '=' ||
    c == '+' || c == '-' || c == '*' ||
    c == '/' || c == '%' || c == '!' ||
    c == '<' || c == '>' || c == '\n';
}

/**
 *  Determines if the given string is a keyword
 *
 *  @param s the string to check
 *
 *  @return true if the string is a keyword, false otherwise
 */
bool Scanner::is_keyword(std::string s) {
    return
    s == "var" || s == "let" || s == "if" ||
    s == "else" || s == "while" || s == "print" ||
    s == "scan";
}

/**
 *  Handles parsing the source code to create an integer
 *
 *  @return the token representing the integer
 */
Token Scanner::handle_integer() {
    Token t;
    int value = 0;
    
    // assign position
    t.line_position = line_number;
    t.col_position = col_number;
    
    while (position < source_length && is_digit(source[position])) {
        value = 10 * value + source[position] - '0';
        position++;
        col_number++;
    }
    
    t.kind = INTEGER;
    t.value = value;
    
    return t;
}

/**
 *  Handles parsing the source code to create an identifier
 *
 *  @return the token representing the identifier
 */
Token Scanner::handle_identifier() {
    Token t;
    std::string s = "";
    
    // assign position
    t.line_position = line_number;
    t.col_position = col_number;
    
    while (position < source_length && (is_letter(source[position]) || is_digit(source[position]))) {
        s += source[position];
        position++;
        col_number++;
    }
    
    // check if s is a keyword
    if (is_keyword(s)) {
        return handle_keyword(t, s);
    }
    
    // return identifier - try putting this above the is_keyword thing
    // and pass a reference into the keyword so it'll change it if needed,
    // then just return t out here
    t.kind = IDENTIFIER;
    t.identifier = s;
    return t;
}

/**
 *  Handles checking if a string is a keyword
 *
 *  @param t the token currently containing the identifier
 *  @param s the string to compare to the list of keywords
 *
 *  @return the altered token containing the new keyword
 */
Token Scanner::handle_keyword(Token t, std::string s) {
    
    if (s == "var") {
        t.kind = VAR;
    } else if (s == "let") {
        t.kind = LET;
    } else if (s == "if") {
        t.kind = IF;
    } else if (s == "else") {
        t.kind = ELSE;
    } else if (s == "while") {
        t.kind = WHILE;
    } else if (s == "print") {
        t.kind = PRINT;
    } else if (s == "scan") {
        t.kind = SCAN;
    }
    
    return t;
}

/**
 *  Handles parsing the source code to create a symbol
 *
 *  @return the token representing the symbol
 */
Token Scanner::handle_symbol() {
    Token t;
    
    t.line_position = line_number;
    t.col_position = col_number;
    
    switch (source[position]) {
        case '(': t.kind = OPEN_PAREN; break;
        case ')': t.kind = CLOSE_PAREN; break;
        case '{': t.kind = OPEN_CURLY; break;
        case '}': t.kind = CLOSE_CURLY; break;
        case ';': t.kind = SEMI_COLON; break;
        case '=':
            if (position < source_length - 1 && source[position + 1] == '=') {
                t.kind = EQUALS;
                position++;
                col_number++;
            } else {
                t.kind = ASSIGN;
            }
            break;
        case '+': t.kind = PLUS; break;
        case '-': t.kind = MINUS; break;
//        case '-':
//            if (position < source_length - 1 && is_digit(source[position + 1])) {
//                position++;
//                Token digit = handle_integer();
//                t.kind = INTEGER;
//                t.value = -digit.value;
//            } else {
//                t.kind = MINUS; break;
//            }
        case '*': t.kind = MULTIPLY; break;
        case '/': t.kind = DIVIDE; break;
        case '%': t.kind = MODULO; break;
        case '!':
            if (position < source_length - 1 && source[position + 1] == '=') {
                t.kind = NOT_EQUALS;
                position++;
                col_number++;
            } else {
                ErrorHandler::illegal_character(true, '!', line_number, col_number);
            }
            break;
        case '<':
            if (position < source_length - 1 && source[position + 1] == '=') {
                
                t.kind = LESS_THAN_EQUALS;
                position++;
                col_number++;
            } else {
                t.kind = LESS_THAN;
            }
            break;
            
        case '>':
            if (position < source_length - 1 && source[position + 1] == '=') {
                
                t.kind = GREATER_THAN_EQUALS;
                position++;
                col_number++;
            } else {
                t.kind = GREATER_THAN;
            }
            break;
        case '\n':
            // don't create a new_line token if no content on the line
            if (!content_flag) {
                line_number++;
                content_flag = false;
                col_number = 1;
                position++;
                return next();
            }
            
            t.kind = NEW_LINE;
            line_number++;
            content_flag = false;
            col_number = 0; // this will be incremented before we return
            break;
            
        default: break;
    }
    
    // gauranteed we always move at least one position
    position++;
    col_number++;
    
    return t;
}

/**
 *  Handles parsing the source code for single line comments
 */
void Scanner::handle_single_comment() {
    while (position < source_length) {
        
        // if next character is newline, leave and return next
        // this will only make the new line if there was content
        // before the comment started
        if (position < source_length - 1 && is_new_line(source[position + 1])) {
            position++;
            col_number++;
            return;
        }
        position++;
        col_number++;
    }
}

/**
 *  Handles parsing the source code for block comments
 */
void Scanner::handle_block_comments() {
    // use the is_new_line but don't make token
    int open_comments = 1;
    
    int start_line = line_number;
    int start_col = col_number - 2; // since we moved ahead two
    
    while (position < source_length) {
        char c = source[position];
        
        if (is_new_line(c)) {
            line_number++;
            content_flag = false;
            col_number = 1;
            position++;
            continue;
        }
        
        if (position < source_length - 1 && c == '*' && source[position + 1] == '/') {
            open_comments--;
            position += 2;
            col_number += 2;
        } else if (position < source_length - 1 && c == '/' && source[position + 1] == '*') {
            open_comments++;
            position += 2;
            col_number += 2;
        } else {
            position++;
            col_number++;
        }
        
        if (open_comments == 0) {
            break;
        }
    }
    
    if (open_comments != 0) {
        ErrorHandler::unterimated_comment(true, start_line, start_col);
    }
}
