//
//  expression_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef expression_node_h
#define expression_node_h

#include "node.h"
#include "type.h"

enum expression_kind {
    NUMBER_LITERAL,
    BOOLEAN_LITERAL,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    LOCATION_EXPRESSION
};

class ExpressionNode: public Node {
public:
    ExpressionNode() : type(nullptr) {}
    ~ExpressionNode() {} // see if type points to symbol_table
    node_kind get_node_kind() { return EXPRESSION_NODE; }
    virtual expression_kind get_expression_kind() = 0;
    
    Type * type;
};

#endif /* expression_node_h */
