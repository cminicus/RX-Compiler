//
//  entry.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef entry_h
#define entry_h

enum entry_kind {
    CONSTANT_ENTRY = 0,
    VARIABLE_ENTRY,
    
    TYPE_ENTRY
};

class Entry {
public:
    virtual ~Entry() {};
    virtual entry_kind get_entry_kind() = 0;
    int line_position;
    int col_position;
};

#endif /* entry_h */
