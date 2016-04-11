//
//  declaration_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef declaration_node_h
#define declaration_node_h

#include "instruction_node.h"
#include "location_node.h"

class DeclarationNode: public InstructionNode {
public:
    DeclarationNode(LocationNode * l, ExpressionNode * e) : location(l), expression(e) {}
    ~DeclarationNode() { delete location; delete expression; }
    instruction_kind get_instruction_kind() { return DECLARATION_INSTRUCTION; }
    
    LocationNode * location;
    ExpressionNode * expression;
};

#endif /* declaration_node_h */
