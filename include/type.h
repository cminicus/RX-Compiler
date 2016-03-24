//
//  type.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef type_h
#define type_h

#include "entry.h"

enum type_kind {
    INTEGER_TYPE = 0
};

class Type: public Entry {
public:
    virtual ~Type() {};
    entry_kind get_kind() { return TYPE_ENTRY; }
    virtual type_kind get_type_kind() = 0;
};

#endif /* type_h */
