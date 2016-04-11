//
//  assign_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef assign_node_h
#define assign_node_h

#include "instruction_node.h"
#include "location_node.h"

class AssignNode: public InstructionNode {
public:
    AssignNode() : location(nullptr), expression(nullptr) {}
    ~AssignNode() { delete location; delete expression; }
    instruction_kind get_instruction_kind() { return ASSIGN_INSTRUCTION; }
    
    // check in compilers for this
    LocationNode * location;
    ExpressionNode * expression;
};

#endif /* assign_node_h */
