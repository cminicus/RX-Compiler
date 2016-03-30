//
//  location_node.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/12/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef location_node_h
#define location_node_h

#include "expression_node.h"

enum location_kind {
    VARIABLE_LOCATION,
    CONSTANT_LOCATION
};

class LocationNode: public ExpressionNode {
public:
    ~LocationNode() {}
    expression_kind get_expression_kind() { return LOCATION_EXPRESSION; };
    virtual location_kind get_location_kind() = 0;
};

#endif /* location_node_h */
