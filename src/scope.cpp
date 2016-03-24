//
//  scope.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "scope.h"
#include <iostream>

Scope::Scope() {
    outer = nullptr;
}

Scope::~Scope() {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        
        // don't try and delete singleton classes
        if (it->second->get_entry_kind() == TYPE_ENTRY) {
            
            Type *t = dynamic_cast<Type *>(it->second);
            if (t->get_type_kind() == INTEGER_TYPE) {
                continue;
            }
        }
        
        delete it->second;
    }
    entries.clear();
    
    for (auto it = inner_scopes.begin(); it != inner_scopes.end(); ++it) {
        delete *it;
    }
    inner_scopes.clear();
}

void Scope::insert(std::string name, Entry *value) {
    std::pair<std::string, Entry *> p(name, value);
    entries.insert(p);
}

Entry* Scope::find(std::string name) {
    Scope *s = this;
    std::map<std::string, Entry *>::iterator it;
    
    while (s != nullptr) {
        it = s->entries.find(name);
        if (it != s->entries.end()) {
            return it->second;
        }
        s = s->outer;
    }
    return nullptr;
}

bool Scope::local(std::string name) {
    std::map<std::string, Entry *>::iterator it;
    
    it = entries.find(name);
    if (it != entries.end()) {
        return true;
    }
    return false;
}

void Scope::add_innner_scope(Scope *scope) {
    inner_scopes.push_back(scope);
}
