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
        case SCAN_ONLY: {
            Scanner scanner(source);
            break;
        }
        case PARSE_ONLY: {
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
            
            LLVMGenerator generator(ast);
            
            // generate code
            std::string code = generator.generate();
            std::cout << code << std::endl;
            
            return code;
        }
        case BUILD: {
            Scanner scanner(source);
            SymbolTable symbol_table;
            AST ast;
            
            Parser parser(scanner, &symbol_table, &ast);
            parser.parse();
            
            LLVMGenerator generator(ast);
            
            // generate code
            std::string code = generator.generate();
            
            // output code to .ll file
            std::ofstream("test.ll") << code;
            
            // compile IR to native architecture
            std::system("llc test.ll");
            
            // compile assembly to executable
            std::system("gcc test.s");
            
            // remove left over .ll file
            std::system("rm test.ll && rm test.s");
            
            return code;
        }
        case RUN: {
            Scanner scanner(source);
            SymbolTable symbol_table;
            AST ast;
            
            Parser parser(scanner, &symbol_table, &ast);
            parser.parse();
            
            LLVMGenerator generator(ast);
            
            // generate code
            std::string code = generator.generate();
            
            // output code to .ll file
            std::ofstream("test.ll") << code;
            
            // compile IR to native architecture
            std::system("llc test.ll");
            
            // compile assembly to executable
            std::system("gcc test.s");
            
            // run executable
            std::system("./a.out");
            
            // remove left over .ll file
            std::system("rm test.ll && rm test.s");
            
            return code;
        }
    }
    
    return "";
}