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
    
    static Boolean * Instance();
    static void DeleteInstance();
    entry_kind get_entry_kind() { return TYPE_ENTRY; };
    type_kind get_type_kind() { return BOOLEAN_TYPE; };
    
private:
    Boolean();
    static Boolean * s_instance;
};

#endif /* boolean_h */
