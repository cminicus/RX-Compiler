//
//  unary_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 5/5/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef unary_node_h
#define unary_node_h

#include "expression_node.h"
#include "token.h"

class UnaryNode: public ExpressionNode {
public:
    UnaryNode(token_kind k, ExpressionNode * expression) : operation(k), expression(expression) {
        type = expression->type;
    }
    
    ~UnaryNode() { delete expression; }
    expression_kind get_expression_kind() { return UNARY_EXPRESSION; };
    
    token_kind operation;
    ExpressionNode * expression;
};

#endif /* unary_node_h */
