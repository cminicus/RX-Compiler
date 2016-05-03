//
//  llvm_generator.h
//  RX Compiler
//
//  Created by Clayton Minicus on 5/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef llvm_generator_h
#define llvm_generator_h

#include "ast.h"
#include <string>
#include <map>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

class LLVMGenerator {
public:
    
    LLVMGenerator(AST &);
    std::string generate();
    
private:
    AST & ast;
    llvm::Module * module;
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;
    
    std::map<Entry *, llvm::AllocaInst *> allocations;
    
    void generate_ast();
    
    llvm::Value * generate_condition(ConditionNode *);
    void generate_block(BlockNode *);
    
    void generate_instruction(InstructionNode *);
    void generate_if(IfNode *);
    void generate_while(WhileNode *);
    void generate_assign(AssignNode *);
    void generate_declaration(DeclarationNode *);
    void generate_print(PrintNode *);
    void generate_scan(ScanNode *);
    
    llvm::Value * generate_expression(ExpressionNode *);
    llvm::Value * generate_number(NumberNode *);
    llvm::Value * generate_binary(BinaryNode *);
    llvm::Value * generate_location(LocationNode *);
    llvm::Value * generate_variable(VariableNode *);
    llvm::Value * generate_constant(ConstantNode *);
};

#endif /* llvm_generator_h */
