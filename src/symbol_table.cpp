//
//  symbol_table.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "symbol_table.h"

SymbolTable::SymbolTable() {
    universe = new Scope;
    universe->insert("int", Integer::Instance());
    
    current_scope = new Scope;
    current_scope->outer = universe;
}

SymbolTable::~SymbolTable() {
    delete universe;
    delete current_scope;
    Integer::DeleteInstance();
}

void SymbolTable::create_scope() {
    Scope *s = new Scope;
    s->outer = current_scope;
    current_scope = s;
}

void SymbolTable::close_scope() {
    Scope *inner = current_scope;
    current_scope = inner->outer;
    inner->outer = nullptr;
    current_scope->add_innner_scope(inner);
}

void SymbolTable::insert(std::string name, Entry *entry) {
    current_scope->insert(name, entry);
}

Entry * SymbolTable::find(std::string name) {
    return current_scope->find(name);
}

std::string SymbolTable::find_name(Entry * entry) {
    return current_scope->find_name(entry);
}

bool SymbolTable::local(std::string name) {
    return current_scope->local(name);
}
