//
//  binary_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef binary_node_h
#define binary_node_h

#include "expression_node.h"
#include "token.h"

class BinaryNode: public ExpressionNode {
public:
    BinaryNode(token_kind k, ExpressionNode * l, ExpressionNode * r) :
        operation(k), left(l), right(r) {
            type = l->type;
        }
    
    ~BinaryNode() { delete left; delete right; }
    expression_kind get_expression_kind() { return BINARY_EXPRESSION; };
    
    token_kind operation;
    ExpressionNode * left;
    ExpressionNode * right;
};

#endif /* binary_node_h */
