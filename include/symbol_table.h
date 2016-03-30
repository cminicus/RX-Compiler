//
//  symbol_table.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef symbol_table_h
#define symbol_table_h

#include "scope.h"
#include "integer.h"
#include "constant.h"
#include "variable.h"

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    
    void create_scope();
    void close_scope();
    
    void insert(std::string, Entry *);
    Entry * find(std::string);
    std::string find_name(Entry *);
    bool local(std::string);
    
private:
    Scope *universe;
    Scope *current_scope;
};

#endif /* symbol_table_h */
