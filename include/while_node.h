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

class WhileNode: public InstructionNode {
public:
    WhileNode() : condition(nullptr), instructions(nullptr) {}
    ~WhileNode() { delete condition; delete instructions; }
    instruction_kind get_instruction_kind() { return WHILE_INSTRUCTION; }
    
    ConditionNode * condition;
    InstructionNode * instructions;
};

#endif /* while_node_h */
