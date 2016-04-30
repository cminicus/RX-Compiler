//
//  rx_compiler.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef rx_compiler_h
#define rx_compiler_h

#include <string>
#include <fstream>

#include "scanner.h"
#include "parser.h"
#include "code_generator.h"

enum compiler_setting {
    SCAN = 0,
    PARSE,
    PARSE_WITH_SYMBOLS,
    PARSE_WITH_AST,
    GENERATE_CODE
};

class RXCompiler {
public:
    RXCompiler(compiler_setting);
    void set_filename(std::string filename);
    void set_source(std::string source);
    std::string compile();
    
private:
    std::string source;
    compiler_setting setting;
};

#endif /* rx_compiler_h */
