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
    universe->insert("i32", new Integer(32));
    universe->insert("bool", new Boolean);
    
    current_scope = new Scope;
    current_scope->outer = universe;
}

SymbolTable::~SymbolTable() {
    delete universe;
    delete current_scope;
}

Scope * SymbolTable::get_current_scope() {
    return current_scope;
}

void SymbolTable::create_scope() {
    Scope * s = new Scope;
    s->outer = current_scope;
    current_scope = s;
}

Scope * SymbolTable::close_scope() {
    Scope * inner = current_scope;
    current_scope = inner->outer;
//    inner->outer = nullptr; // ? 
//    current_scope->add_innner_scope(inner);
    
    return inner;
}

void SymbolTable::insert(std::string name, Entry * entry) {
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
