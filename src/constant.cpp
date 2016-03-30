//
//  constant.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "constant.h"

Constant::~Constant() {
    if (type != nullptr && type->get_type_kind() != INTEGER_TYPE) {
        delete type;
    }
}