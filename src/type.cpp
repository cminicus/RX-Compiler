//
//  type.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/31/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "type.h"

std::map<type_kind, std::string> Type::mapping = {
    {INTEGER_TYPE, "int"},
    {BOOLEAN_TYPE, "bool"}
};