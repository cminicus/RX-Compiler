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

class BlockNode;

class IfNode: public InstructionNode {
public:
    IfNode() {}
    ~IfNode() {
        for (auto it = conditions.begin(); it != conditions.end(); ++it) {
            delete (*it);
        }
        
//        for (auto it = instructions.begin(); it != instructions.end(); ++it) {
//            delete (*it);
//        }
        
        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
            delete (*it);
        }
    }
    instruction_kind get_instruction_kind() { return IF_INSTRUCTION; }
    
    std::vector<ExpressionNode *> conditions;
    std::vector<BlockNode *> blocks;
};

#endif /* if_node_h */
