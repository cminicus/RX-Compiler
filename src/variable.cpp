//
//  variable.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "variable.h"

Variable::~Variable() {
    if (type->get_type_kind() != INTEGER_TYPE) {
        delete type;
    }
}