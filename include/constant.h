//
//  constant.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef constant_h
#define constant_h

#include "entry.h"
#include "type.h"
#include "integer.h"
#include "boolean.h"
#include "token.h"

class Constant: public Entry {
public:
    Constant() : value(0), type(nullptr) { line_position = -1; col_position = -1; }
    Constant(Token, Type *);
//    ~Constant() { delete type; }
    entry_kind get_entry_kind() { return CONSTANT_ENTRY; };
    
    int value;
    int stack_offset = -1;
    Type * type;
    std::string identifier;
};

#endif /* constant_h */
