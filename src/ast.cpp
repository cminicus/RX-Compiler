//
//  ast.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "ast.h"

AST::AST() : root(nullptr) {}

AST::~AST() {
    if (root != nullptr) {
        delete root;
    }
}

void AST::set_root(Node *root) {
    this->root = root;
}

Node * AST::get_root() {
    return this->root;
}