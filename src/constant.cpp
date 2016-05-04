//
//  constant.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "constant.h"

Constant::Constant(Token token, Type * type) {
    value = token.value;
    line_position = token.line_position;
    col_position = token.col_position;
    
    this->type = type;
    identifier = token.identifier;
}