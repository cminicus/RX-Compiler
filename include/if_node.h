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
#include <vector>

class IfNode: public InstructionNode {
public:
    IfNode() {}
    ~IfNode() {
        for (auto it = conditions.begin(); it != conditions.end(); ++it) {
            delete (*it);
        }
        
        for (auto it = instructions.begin(); it != instructions.end(); ++it) {
            delete (*it);
        }
    }
    instruction_kind get_instruction_kind() { return IF_INSTRUCTION; }
    
    std::vector<ConditionNode *> conditions;
    std::vector<InstructionNode *> instructions;
};

#endif /* if_node_h */
