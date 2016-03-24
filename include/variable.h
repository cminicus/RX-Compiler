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

class Variable: public Entry {
public:
    ~Variable();
    entry_kind get_entry_kind() { return VARIABLE_ENTRY; };
    
    int value;
    Type *type;
};


#endif /* variable_h */
