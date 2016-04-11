//
//  variable_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/12/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef variable_node_h
#define variable_node_h

#include "location_node.h"
#include "variable.h"

class VariableNode: public LocationNode {
public:
    VariableNode(Variable * v) : variable(v) { this->type = v->type; }
    VariableNode() : variable(nullptr) {}
    // Make a ConstantNode? Constants here don't have to resolve at compile time
    ~VariableNode() {}
    location_kind get_location_kind() { return VARIABLE_LOCATION; };
    
    // pointer to symbol table
    Variable * variable;
};

#endif /* variable_node_h */
