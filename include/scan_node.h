//
//  scan_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 5/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef scan_node_h
#define scan_node_h

#include "instruction_node.h"

class ScanNode: public InstructionNode {
public:
    ScanNode() : location(nullptr) {}
    ~ScanNode() { delete location; }
    instruction_kind get_instruction_kind() { return SCAN_INSTRUCTION; }
    
    LocationNode * location;
};

#endif /* scan_node_h */
