//
//  while_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef while_node_h
#define while_node_h

#include "instruction_node.h"

class BlockNode;

class WhileNode: public InstructionNode {
public:
    WhileNode() : condition(nullptr), block(nullptr) {}
    ~WhileNode() { delete condition; delete block; }
    instruction_kind get_instruction_kind() { return WHILE_INSTRUCTION; }
    
    ConditionNode * condition;
//    InstructionNode * instructions;
    BlockNode * block;
};

#endif /* while_node_h */
