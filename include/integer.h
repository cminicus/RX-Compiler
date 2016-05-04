//
//  integer.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef integer_h
#define integer_h

#include "type.h"

class Integer: public Type {
public:
    
    Integer(int bits = 32, bool sign = true) : sign(sign), num_bits(bits) {}
    int get_num_bits() { return num_bits; }
    entry_kind get_entry_kind() { return TYPE_ENTRY; }
    type_kind get_type_kind() const { return INTEGER_TYPE; }
    bool is_signed() { return this->sign; }
    
    bool operator==(const Type & rhs) const {
        if (rhs.get_type_kind() == INTEGER_TYPE) {
            return true;
        }
        return false;
    }
    
    bool operator!=(const Type & rhs) const {
        return !(*this == rhs);
    }

    // maybe have number type? so that ints and floats are auto equal??
    
private:
    bool sign;
    int num_bits;
};

#endif /* integer_h */
