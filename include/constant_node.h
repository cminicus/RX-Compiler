//
//  constant_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/24/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef constant_node_h
#define constant_node_h

#include "location_node.h"
#include "constant.h"

// maybe don't make this a location eventually
// just from expression?
// because locations are places you can assign to?
// or maybe they aren't (like in print) -> look into this

class ConstantNode: public LocationNode {
public:
    ConstantNode() : constant(nullptr) {}
    // Make a ConstantNode? Constants here don't have to resolve at compile time
    ~ConstantNode() {}
    location_kind get_location_kind() { return CONSTANT_LOCATION; };
    
    // pointer to symbol table
    Constant *constant;
};

#endif /* constant_node_h */
