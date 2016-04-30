//
//  variable.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef variable_h
#define variable_h

#include "entry.h"
#include "type.h"
#include "integer.h"
#include "boolean.h"
#include "token.h"

class Variable: public Entry {
public:
    Variable() : value(0), type(nullptr) { line_position = -1; col_position = -1; }
    Variable(Token, Type *);
    ~Variable();
    entry_kind get_entry_kind() { return VARIABLE_ENTRY; };
    
    int value;
    int stack_offset = -1;
    Type * type;
};


#endif /* variable_h */
