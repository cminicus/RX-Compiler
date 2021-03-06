
//
//  scope.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef scope_h
#define scope_h

#include <string>
#include <map>
#include <vector>
#include "entry.h"
#include "type.h"

class Scope {
public:
    Scope();
    ~Scope();
    
    void insert(std::string, Entry *);
    Entry * find(std::string);
    std::string find_name(Entry *);
    bool local(std::string);
    void add_innner_scope(Scope *);
    
    Scope * outer;
    int size = 0;
    std::map<std::string, Entry *> entries;
    
private:
    
    std::vector<Scope *> inner_scopes;
};

#endif /* scope_h */
