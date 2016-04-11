//
//  instruction_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef instruction_node_h
#define instruction_node_h

#include "node.h"

enum instruction_kind {
    IF_INSTRUCTION,
    WHILE_INSTRUCTION,
    ASSIGN_INSTRUCTION,
    PRINT_INSTRUCTION,
    DECLARATION_INSTRUCTION
};

class InstructionNode: public Node {
public:
    InstructionNode() : next(nullptr) {}
    ~InstructionNode() { delete next; }
    node_kind get_node_kind() { return INSTRUCTION_NODE; }
    virtual instruction_kind get_instruction_kind() = 0;
    
    InstructionNode * next;
};

#endif /* instruction_node_h */
