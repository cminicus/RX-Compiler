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

class Constant: public Entry {
public:
    ~Constant();
    entry_kind get_entry_kind() { return CONSTANT_ENTRY; };
    
    int value; // make this const?
    Type *type;
};

#endif /* constant_h */
