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
    
    static Integer * Instance();
    static void DeleteInstance();
    entry_kind get_entry_kind() { return TYPE_ENTRY; }
    type_kind get_type_kind() { return INTEGER_TYPE; }
    int get_type_size() { return 4; }
    
private:
    Integer();
    static Integer * s_instance;
};

#endif /* integer_h */
