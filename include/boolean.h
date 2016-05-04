//
//  boolean.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/31/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef boolean_h
#define boolean_h

#include "type.h"

class Boolean: public Type {
public:
    
    Boolean() {}
    entry_kind get_entry_kind() { return TYPE_ENTRY; }
    type_kind get_type_kind() const { return BOOLEAN_TYPE; }
    
    bool operator==(const Type & rhs) const {
        if (rhs.get_type_kind() == BOOLEAN_TYPE) {
            return true;
        }
        return false;
    }
    
    bool operator!=(const Type & rhs) const {
        return !(*this == rhs);
    }
};

#endif /* boolean_h */
