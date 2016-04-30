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
    INTEGER_TYPE = 0,
    BOOLEAN_TYPE,
};

class Type: public Entry {
public:
    virtual ~Type() {};
    entry_kind get_kind() { return TYPE_ENTRY; }
    virtual type_kind get_type_kind() = 0;
    virtual int get_type_size() = 0;
    
    static std::map<type_kind, std::string> mapping;
};

#endif /* type_h */
