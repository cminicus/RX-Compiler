//
//  llvm_generator.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 5/1/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "llvm_generator.h"

LLVMGenerator::LLVMGenerator(AST & root) : ast(root), Builder(Context) {
    
    module = new llvm::Module("ir.ll", Context);
}

std::string LLVMGenerator::generate() {
    generate_ast();
    
    std::string output;
    llvm::raw_string_ostream output_stream(output);
    output_stream << *module;
    output_stream.flush();
    
    return output;
}

void LLVMGenerator::generate_ast() {
    Node * root = ast.get_root();
    
    
    llvm::FunctionType * main_type = llvm::FunctionType::get(llvm::IntegerType::get(Context, 32), false);
    llvm::Function * main = llvm::Function::Create(
        main_type,
        llvm::GlobalValue::ExternalLinkage,
        "main", module
    );
    main->setCallingConv(llvm::CallingConv::C);
    
    llvm::BasicBlock * bb = llvm::BasicBlock::Create(Context, "entry", main);
    Builder.SetInsertPoint(bb);
    
    
    
    generate_instruction(dynamic_cast<InstructionNode *>(root));
    
    Builder.CreateRet(llvm::ConstantInt::get(Context, llvm::APInt(32, 0)));
//    module->dump();
}

// ------------------------------- Instructions --------------------------------

void LLVMGenerator::generate_instruction(InstructionNode * node) {
    if (node == nullptr) {
        return;
    }
    
    switch (node->get_instruction_kind()) {
        case IF_INSTRUCTION:          generate_if         (dynamic_cast<IfNode *>(node)); break;
        case WHILE_INSTRUCTION:       generate_while      (dynamic_cast<WhileNode *>(node)); break;
        case ASSIGN_INSTRUCTION:      generate_assign     (dynamic_cast<AssignNode *>(node)); break;
        case PRINT_INSTRUCTION:       generate_print      (dynamic_cast<PrintNode *>(node)); break;
        case SCAN_INSTRUCTION:        generate_scan       (dynamic_cast<ScanNode *>(node)); break;
        case DECLARATION_INSTRUCTION: generate_declaration(dynamic_cast<DeclarationNode *>(node)); break;
    }
    
    generate_instruction(node->next);
}

void LLVMGenerator::generate_if(IfNode * node) {
    // get parent function
    llvm::Function * function = Builder.GetInsertBlock()->getParent();

    // create array of blocks
    std::vector<llvm::BasicBlock *> condition_blocks;

    // make blocks for all conditions
    for (int i = 0; i < (int) node->blocks.size(); i++) {
        std::string label = "cond_" + std::to_string(i);
        llvm::BasicBlock * condition_block = llvm::BasicBlock::Create(Context, label);
        condition_blocks.push_back(condition_block);
    }
    
    // make an end block (this will always exist)
    llvm::BasicBlock * end_block  = llvm::BasicBlock::Create(Context, "end_if");
    condition_blocks.push_back(end_block);
    
    for (int i = 0; i < (int) condition_blocks.size() - 1; i++) {

        // only add new block if we know there are predecessors (i > 0)
        // or if there are no predecessors (block list is empty)
        // otherwise we would add an empty basic block for no reason
        if (i > 0 || function->getBasicBlockList().empty()) {
            // set insertion point at first condition block
            Builder.SetInsertPoint(condition_blocks[i]);
            
            // add condition block to function
            function->getBasicBlockList().push_back(condition_blocks[i]);
        }
        
        // create block for if the condition is true
        llvm::BasicBlock * true_block = llvm::BasicBlock::Create(Context, "block_" + std::to_string(i));
        
        // add true block to function
        function->getBasicBlockList().push_back(true_block);
        
        // generate condition if it exists
        if (i < (int) node->conditions.size()) {
            llvm::Value * condition = generate_condition(node->conditions[i]);
            
            // create conditional branch
            Builder.CreateCondBr(condition, true_block, condition_blocks[i + 1]);
        } else {
            // for else statements
            Builder.CreateBr(true_block);
        }

        // set insertion point at true block
        Builder.SetInsertPoint(true_block);
        
        // generate true instructions
        generate_block(node->blocks[i]);
        // jump to end
        Builder.CreateBr(end_block);
    }

    // add end block
    function->getBasicBlockList().push_back(end_block);
    // set end block as insertion point
    Builder.SetInsertPoint(end_block);
}

void LLVMGenerator::generate_while(WhileNode * node) {
    // get parent function
    llvm::Function * function = Builder.GetInsertBlock()->getParent();
    
    llvm::BasicBlock * loop = llvm::BasicBlock::Create(Context, "while_loop");
    llvm::BasicBlock * end = llvm::BasicBlock::Create(Context, "while_loop_end");
    
    // generate condition
    llvm::Value * condition = generate_condition(node->condition);
    // create initial branch
    Builder.CreateCondBr(condition, loop, end);
    
    function->getBasicBlockList().push_back(loop);
    
    // generate loop
    Builder.SetInsertPoint(loop);
    generate_block(node->block);
    
    // generate condition again
    llvm::Value * next_condition = generate_condition(node->condition);
    // create next branch
    Builder.CreateCondBr(next_condition, loop, end);
    
    function->getBasicBlockList().push_back(end);
    Builder.SetInsertPoint(end);
}

void LLVMGenerator::generate_assign(AssignNode * node) {
    llvm::Value * location = generate_location(node->location);
    llvm::Value * expression = generate_expression(node->expression);
    
    if (node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        expression = Builder.CreateLoad(expression);
    }
    
    Builder.CreateStore(expression, location);
}

void LLVMGenerator::generate_declaration(DeclarationNode * node) {
    llvm::Value * location = generate_location(node->location);
    llvm::Value * expression = generate_expression(node->expression);
    
    if (node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        expression = Builder.CreateLoad(expression);
    }
    
    Builder.CreateStore(expression, location);
}

void LLVMGenerator::generate_print(PrintNode * node) {
    llvm::Value * expression = generate_expression(node->expression);
    
    if (node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        expression = Builder.CreateLoad(expression);
    }
    
    llvm::Function * _printf = module->getFunction("printf");
    if (!_printf) {
        llvm::FunctionType * _printf_type = llvm::FunctionType::get(
            llvm::IntegerType::get(Context, 32), // return type
            { llvm::PointerType::get(llvm::IntegerType::get(Context, 8), 0) }, // arguments
            true // varags
        );
        _printf = llvm::Function::Create(_printf_type, llvm::GlobalValue::ExternalLinkage, "printf", module);
        _printf->setCallingConv(llvm::CallingConv::C);
    }
    
    // FIXME: Why doesn't this work?
    llvm::GlobalVariable * int_format = module->getGlobalVariable("printf_int_format");
    if (!int_format) {
        llvm::Constant * int_string = llvm::ConstantDataArray::getString(Context, "%d\x0A");
        int_format = new llvm::GlobalVariable(*module, int_string->getType(), true, llvm::GlobalVariable::PrivateLinkage, int_string, "printf_int_format");
    }
    
    llvm::Constant * zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(Context));
    llvm::Constant * indices[] {zero, zero};
    llvm::Constant * string_value = llvm::ConstantExpr::getGetElementPtr(int_format, indices, true);
    
    std::vector<llvm::Value *> actuals = {
        string_value,
        expression
    };
    
    Builder.CreateCall(_printf, actuals);
}

void LLVMGenerator::generate_scan(ScanNode * node) {
    llvm::Value * location = generate_location(node->location);
    
    llvm::Function * _scanf = module->getFunction("scanf");
    if (!_scanf) {
        llvm::FunctionType * _scanf_type = llvm::FunctionType::get(
            llvm::IntegerType::get(Context, 32), // return type
            { llvm::PointerType::get(llvm::IntegerType::get(Context, 8), 0) }, // arguments
            true // varags
        );
        _scanf = llvm::Function::Create(_scanf_type, llvm::GlobalValue::ExternalLinkage, "scanf", module);
        _scanf->setCallingConv(llvm::CallingConv::C);
    }
    
    // FIXME: Why doesn't this work?
    llvm::GlobalVariable * int_format = module->getGlobalVariable("scanf_int_format");
    if (!int_format) {
        llvm::Constant * int_string = llvm::ConstantDataArray::getString(Context, "%d");
        int_format = new llvm::GlobalVariable(*module, int_string->getType(), true, llvm::GlobalVariable::PrivateLinkage, int_string, "scanf_int_format");
    }
    
    llvm::Constant * zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(Context));
    llvm::Constant * indices[] {zero, zero};
    llvm::Constant * string_value = llvm::ConstantExpr::getGetElementPtr(int_format, indices, true);
    
    std::vector<llvm::Value *> actuals = {
        string_value,
        location
    };
    
    Builder.CreateCall(_scanf, actuals);
}

// -------------------------------- Conditions ---------------------------------

llvm::Value * LLVMGenerator::generate_condition(ConditionNode * node) {
    llvm::Value * left = generate_expression(node->left);
    llvm::Value * right = generate_expression(node->right);
    
    
    if (node->left->get_expression_kind() == LOCATION_EXPRESSION) {
        left = Builder.CreateLoad(left);
    }
    
    if (node->right->get_expression_kind() == LOCATION_EXPRESSION) {
        right = Builder.CreateLoad(right);
    }
    
    switch (node->operation) {
        case EQUALS:              return Builder.CreateICmpEQ (left, right, "==");
        case NOT_EQUALS:          return Builder.CreateICmpNE (left, right, "!=");
        case LESS_THAN:           return Builder.CreateICmpSLT(left, right, "<");
        case LESS_THAN_EQUALS:    return Builder.CreateICmpSLE(left, right, "<=");
        case GREATER_THAN:        return Builder.CreateICmpSGT(left, right, ">");
        case GREATER_THAN_EQUALS: return Builder.CreateICmpSGE(left, right, ">=");
        default:                  return nullptr;
    }
}

// ---------------------------------- Blocks -----------------------------------

void LLVMGenerator::generate_block(BlockNode * node) {
    generate_instruction(node->instructions);
}

// -------------------------------- Expressions --------------------------------

llvm::Value * LLVMGenerator::generate_expression(ExpressionNode * node) {
    switch (node->get_expression_kind()) {
        case NUMBER_EXPRESSION:   return generate_number  (dynamic_cast<NumberNode *>(node)); break;
        case BINARY_EXPRESSION:   return generate_binary  (dynamic_cast<BinaryNode *>(node)); break;
        case LOCATION_EXPRESSION: return generate_location(dynamic_cast<LocationNode *>(node)); break;
    }
}

llvm::Value * LLVMGenerator::generate_number(NumberNode * node) {
    // create value
    llvm::APInt value = llvm::APInt(32, node->constant->value, true);
    
    // return integer
    return llvm::ConstantInt::get(module->getContext(), value);
}

llvm::Value * LLVMGenerator::generate_binary(BinaryNode * node) {
    llvm::Value * left = generate_expression(node->left);
    llvm::Value * right = generate_expression(node->right);
    
    
    if (node->left->get_expression_kind() == LOCATION_EXPRESSION) {
        left = Builder.CreateLoad(left);
    }
    
    if (node->right->get_expression_kind() == LOCATION_EXPRESSION) {
        right = Builder.CreateLoad(right);
    }
    
    switch (node->operation) {
        case PLUS:     return Builder.CreateAdd (left, right, "add");
        case MINUS:    return Builder.CreateSub (left, right, "sub");
        case MULTIPLY: return Builder.CreateMul (left, right, "mul");
        case DIVIDE:   return Builder.CreateSDiv(left, right, "div");
        case MODULO:   return Builder.CreateSRem(left, right, "mod");
        default:       return nullptr;
    }
}

llvm::Value * LLVMGenerator::generate_location(LocationNode * node) {
    switch (node->get_location_kind()) {
        case VARIABLE_LOCATION: return generate_variable(dynamic_cast<VariableNode *>(node)); break;
        case CONSTANT_LOCATION: return generate_constant(dynamic_cast<ConstantNode *>(node)); break;
    }
}

llvm::Value * LLVMGenerator::generate_variable(VariableNode * node) {
    
    // check if allocation exists
    if (!allocations[node->variable]) {
        allocations[node->variable] = Builder.CreateAlloca(llvm::IntegerType::get(module->getContext(), 32));
    }
    
    return allocations[node->variable] ;
}

llvm::Value * LLVMGenerator::generate_constant(ConstantNode * node) {
    
    // check if allocation exists
    if (!allocations[node->constant]) {
        // create/save allocation if not
        allocations[node->constant] = Builder.CreateAlloca(llvm::IntegerType::get(module->getContext(), 32));
    }
    
    return allocations[node->constant];
}