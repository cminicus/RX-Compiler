//
//  rx_compiler.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/11/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "rx_compiler.h"

RXCompiler::RXCompiler(compiler_setting setting) {
    this->source = "";
    this->setting = setting;
}

void RXCompiler::set_filename(std::string filename) {
    std::ifstream stream(filename);
    std::string file((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    this->source = file;
}

void RXCompiler::set_source(std::string source) {
    this->source = source;
}

std::string RXCompiler::compile() {
    switch (setting) {
        case SCAN: {
            Scanner scanner(source);
            break;
        }
        case PARSE: {
            Scanner scanner(source);
            
            Parser parser(scanner);
            parser.parse();
            break;
        }
        case PARSE_WITH_SYMBOLS: {
            Scanner scanner(source);
            SymbolTable symbol_table;
            
            Parser parser(scanner, &symbol_table);
            parser.parse();
            break;
        }
        case PARSE_WITH_AST: {
            Scanner scanner(source);
            SymbolTable symbol_table;
            AST ast;
            
            Parser parser(scanner, &symbol_table, &ast);
            parser.parse();
            break;
        }
        case GENERATE_CODE: {
            Scanner scanner(source);
            SymbolTable symbol_table;
            AST ast;
            
            Parser parser(scanner, &symbol_table, &ast);
            parser.parse();
            
            CodeGenerator generator(symbol_table, ast);
            // set backend?
            std::string code = generator.generate();
//            std::cout << code << std::endl;
            
            return code;
        }
    }
    
    return "";
}