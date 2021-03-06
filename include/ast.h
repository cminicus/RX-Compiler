//
//  ast.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef ast_h
#define ast_h

#include "block_node.h"
#include "if_node.h"
#include "while_node.h"
#include "assign_node.h"
#include "print_node.h"
#include "scan_node.h"
#include "declaration_node.h"
#include "expression_node.h"
#include "number_node.h"
#include "boolean_node.h"
#include "binary_node.h"
#include "unary_node.h"
#include "variable_node.h"
#include "constant_node.h"

class AST {
public:
    AST();
    ~AST();
    void set_root(Node *);
    Node * get_root();
    
private:
    Node * root;
};

#endif /* ast_h */
