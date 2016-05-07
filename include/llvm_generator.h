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

#include <llvm/PassManager.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

class LLVMGenerator {
public:
    
    LLVMGenerator(AST &);
//    ~LLVMGenerator
    std::string generate();
    
private:
    AST & ast;
    llvm::Module * module;
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;
    llvm::legacy::PassManager * PM;
    
    std::map<Entry *, llvm::AllocaInst *> allocations;
    
    void initializePassManager();
    
    void generate_ast();
    
    void generate_block(BlockNode *);
    
    llvm::Value * generate_expression(ExpressionNode *);
    llvm::Value * generate_number(NumberNode *);
    llvm::Value * generate_boolean(BooleanNode *);
    llvm::Value * generate_binary(BinaryNode *);
    llvm::Value * generate_unary(UnaryNode *);
    llvm::Value * generate_location(LocationNode *);
    llvm::Value * generate_variable(VariableNode *);
    llvm::Value * generate_constant(ConstantNode *);
    
    llvm::Value * generate_instruction(InstructionNode *);
    llvm::Value * generate_if(IfNode *);
    llvm::Value * generate_while(WhileNode *);
    llvm::Value * generate_assign(BinaryNode *);
    llvm::Value * generate_declaration(DeclarationNode *);
    llvm::Value * generate_print(PrintNode *);
    llvm::Value * generate_scan(ScanNode *);
    
    // utility functions
    llvm::Type * get_llvm_type(Type *);
    void convert_boolean_to_comparison(llvm::Value **, ExpressionNode *);
    void store_comparison_in_boolean(llvm::CmpInst *, llvm::Value *);
};

#endif /* llvm_generator_h */
