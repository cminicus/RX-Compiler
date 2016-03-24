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

class NumberNode: public ExpressionNode {
public:
    NumberNode() : constant(nullptr) {}
    ~NumberNode() { delete constant; }
    expression_kind get_expression_kind() { return NUMBER_EXPRESSION; };
    
    Constant *constant;
    // should we make a constant object here? It's not actually in the
    // symbol table, so we could just have the value and the type here
    // but maybe it's best so we don't do that in multiple places as more
    // types come in
    // so delete this in the destructor -> it won't be used elsewhere
};

#endif /* number_node_h */
