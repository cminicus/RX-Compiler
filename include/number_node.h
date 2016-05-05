//
//  number_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef number_node_h
#define number_node_h

#include "expression_node.h"
#include "constant.h"

// Represents a boolean literal
class NumberNode: public ExpressionNode {
public:
    NumberNode(Constant * c) : constant(c) { this->type = c->type; }
    ~NumberNode() { delete constant; }
    expression_kind get_expression_kind() { return NUMBER_LITERAL; };
    
    Constant * constant;
};

#endif /* number_node_h */
