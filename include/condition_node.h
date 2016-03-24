//
//  condition_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef condition_node_h
#define condition_node_h

#include "node.h"
#include "token.h"
#include "expression_node.h"

class ConditionNode: public Node {
public:
    ConditionNode() : left(nullptr), right(nullptr) {}
    ~ConditionNode() { delete left; delete right; }
    node_kind get_node_kind() { return CONDITION_NODE; }
    token_kind operation = NONE;
    
    ExpressionNode *left;
    ExpressionNode *right;
};

#endif /* condition_node_h */
