//
//  block_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 4/20/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef block_node_h
#define block_node_h

#include "node.h"
#include "instruction_node.h"
#include "scope.h"

class BlockNode: public Node {
public:
    BlockNode() {}
    ~BlockNode() { delete instructions; delete scope; }
    node_kind get_node_kind() { return BLOCK_NODE; }
    
    InstructionNode * instructions = nullptr;
    Scope * scope = nullptr;
};

#endif /* block_node_h */
