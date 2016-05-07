//
//  instruction_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef instruction_node_h
#define instruction_node_h

#include "expression_node.h"

enum instruction_kind {
    IF_INSTRUCTION,
    WHILE_INSTRUCTION,
    ASSIGN_INSTRUCTION,
    PRINT_INSTRUCTION,
    SCAN_INSTRUCTION,
    DECLARATION_INSTRUCTION
};

//class InstructionNode: public Node {
class InstructionNode: public ExpressionNode {
public:
    InstructionNode() {}
    expression_kind get_expression_kind() { return INSTRUCTION_EXPRESSION; }
    virtual instruction_kind get_instruction_kind() = 0;
};

#endif /* instruction_node_h */
