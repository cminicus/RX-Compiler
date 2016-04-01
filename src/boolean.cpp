//
//  boolean.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/31/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "boolean.h"

Boolean * Boolean::s_instance;

Boolean::Boolean() {}

void Boolean::DeleteInstance() {
    delete s_instance;
    s_instance = nullptr;
}

Boolean * Boolean::Instance() {
    if (s_instance == nullptr) {
        s_instance = new Boolean;
    }
    return s_instance;
}