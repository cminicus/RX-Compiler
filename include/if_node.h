//
//  if_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef if_node_h
#define if_node_h

#include "instruction_node.h"

class IfNode: public InstructionNode {
public:
    IfNode() : condition(nullptr), true_instructions(nullptr), false_instructions(nullptr) {}
    ~IfNode() { delete condition; delete true_instructions; delete false_instructions; }
    instruction_kind get_instruction_kind() { return IF_INSTRUCTION; }
    
    ConditionNode *condition;
    InstructionNode *true_instructions;
    InstructionNode *false_instructions;
};

#endif /* if_node_h */
