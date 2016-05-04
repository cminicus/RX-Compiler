//
//  boolean_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 5/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef boolean_node_h
#define boolean_node_h

#include "expression_node.h"
#include "constant.h"

// Represents a boolean literal
class BooleanNode: public ExpressionNode {
public:
    BooleanNode(Constant * c) : constant(c) { this->type = c->type; }
    ~BooleanNode() { delete constant; }
    expression_kind get_expression_kind() { return BOOLEAN_EXPRESSION; };
    
    Constant * constant;
};

#endif /* boolean_node_h */
