//
//  integer.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/4/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "integer.h"

Integer * Integer::s_instance;

Integer::Integer() {}

void Integer::DeleteInstance() {
    delete s_instance;
    s_instance = nullptr;
}

Integer * Integer::Instance() {
    if (s_instance == nullptr) {
        s_instance = new Integer;
    }
    return s_instance;
}