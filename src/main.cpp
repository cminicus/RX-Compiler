//
//  main.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "rx_compiler.h"

using std::cout;
using std::endl;
using std::cerr;

// ------------------------- Function Declarations -----------------------------
std::string handle_arguments(int, const char * []);
void compile(std::string);

// ------------------------------ Functions ------------------------------------
int main(int argc, const char * argv[]) {
    
    try {
        string filename = handle_arguments(argc, argv);
        compile(filename);
        
    } catch (std::runtime_error & error) {
        cerr << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

std::string handle_arguments(int argc, const char * argv[]) {
    if (argc != 2) {
        throw std::runtime_error("missing filename\nuseage: ./compiler filename");
    }
    return argv[1];
}

void compile(std::string filename) {
    RXCompiler compiler(GENERATE_CODE);
    compiler.set_filename(filename);
    compiler.compile();
}
