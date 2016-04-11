//
//  print_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef print_node_h
#define print_node_h

#include "instruction_node.h"

class PrintNode: public InstructionNode {
public:
    PrintNode() : location(nullptr) {}
    ~PrintNode() { delete location; }
    instruction_kind get_instruction_kind() { return PRINT_INSTRUCTION; }
    
    LocationNode * location;
};

#endif /* print_node_h */
