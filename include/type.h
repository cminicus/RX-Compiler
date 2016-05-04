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
#include <map>
#include <string>

enum type_kind {
    INTEGER_TYPE = 0, // NUMBER_TYPE -> i8-i64, u8-u64, f32-f64
    BOOLEAN_TYPE,
    // CHARACTER_TYPE
    // STRING_TYPE -> this is an array of characters, so probably don't need it
    // ARRAY_TYPE
    // STRUCT_TYPE
    // CLASS_TYPE
};

class Type: public Entry {
public:
    virtual ~Type() {};
    entry_kind get_kind() { return TYPE_ENTRY; }
    virtual type_kind get_type_kind() const = 0;
    
    static std::map<type_kind, std::string> mapping;
    
    virtual bool operator==(const Type & rhs) const = 0;
    virtual bool operator!=(const Type & rhs) const = 0;
};

#endif /* type_h */
