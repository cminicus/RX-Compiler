//
//  node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef node_h
#define node_h

enum node_kind {
    INSTRUCTION_NODE,
    EXPRESSION_NODE,
    CONDITION_NODE,
    BLOCK_NODE
};

class Node {
public:
    virtual ~Node() {}
    virtual node_kind get_node_kind() = 0;
};

#endif /* node_h */
