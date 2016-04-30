//
//  code_generator.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/30/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef code_generator_h
#define code_generator_h

#include <sstream>
#include <assert.h>
#include "symbol_table.h"
#include "ast.h"

#include "registers.h"

#define printf "_printf"

struct exp_packet {
    bool is_constant;
    int value = -1;
};

class CodeGenerator {
public:
    CodeGenerator(SymbolTable & table, AST & ast) : symbol_table(table), ast(ast) {}
    std::string generate();

private:
    SymbolTable & symbol_table;
    AST & ast;
    
    int current_stack_offset = 0;
    
    const std::string int_format = "int_format";
    bool int_format_added = false;
    
    std::stringstream program;
    std::stringstream formats;
    std::string tab = "    ";
    
    void parse_symbol_table();
    void parse_scope(Scope *);
    void end_scope(Scope *);
    std::string parse_ast();
    
    void program_setup();
    
    exp_packet generate_condition(ConditionNode *);
    void generate_block(BlockNode *);
    
    void generate_instruction(InstructionNode *);
    void generate_if(IfNode *);
    void generate_while(WhileNode *);
    void generate_assign(AssignNode *);
    void generate_print(PrintNode *);
    void generate_declaration(DeclarationNode *);
    
    exp_packet generate_expression(ExpressionNode *);
    exp_packet generate_number(NumberNode *);
    exp_packet generate_binary(BinaryNode *);
    exp_packet generate_location(LocationNode *);
    exp_packet generate_variable(VariableNode *);
    exp_packet generate_constant(ConstantNode *);
    
    void parse_location_packet();
    void parse_expression_packet(ExpressionNode *, exp_packet, std::string);
    
    void parse_expression_to_register(exp_packet, ExpressionNode *, std::string, std::string, std::string);
    
    void dereference_if_needed(std::string, ExpressionNode *);
    
    // ----------------------------- Utility Functions -------------------------
    
    // labels
    std::string next_label() {
        static int i = 0;
        return ".L" + std::to_string(i++);
    }
    
    void add_label(std::string label) {
        program << label << ":" << std::endl;
    }
    
    void add_directive(std::string dir) {
        program << dir << "  " << std::endl;
    }
    
    void add_directive(std::string dir, std::string arg) {
        program << dir << "  " << arg << std::endl;
    }
    
    void new_line() {
        program << std::endl;
    }
    
    // utility
    void function_setup() {
        push(rbp);
        move(rsp, rbp);
    }
    
    void function_teardown() {
        move(rbp, rsp);
        pop(rbp);
        ret();
    }
    
    void allocate_stack_space(int size) {
        int aligned_size = round(size, 16);
        sub("$" + std::to_string(aligned_size), rsp);
    }
    
    void deallocate_stack_space(int size) {
        add("$" + std::to_string(size), rsp);
    }
    
    int round(int number, int multiple) {
        int remainder = number % multiple;
        
        if (remainder == 0) {
            return number;
        }
        
        return number + multiple - remainder;
    }
    
    // move/load
    void move(std::string from, std::string to, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "mov     " << from << ", " << to << std::endl;
        } else {
            program << tab << "mov" << postfix << "    " << from << ", " << to << std::endl;
        }
    }
    
    void push(std::string reg, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "push    " << reg << std::endl;
        } else {
            program << tab << "push" << postfix << "   " << reg << std::endl;
        }
    }
    
    void pop(std::string reg, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "pop     " << reg << std::endl;
        } else {
            program << tab << "pop" << postfix << "    " << reg << std::endl;
        }
    }
    
    void load(std::string address, std::string reg) {
        program << tab << "lea     " << address << ", " << reg << std::endl;
    }
    
    // arithmetic
    void add(std::string source, std::string destination, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "add     " << source << ", " << destination << std::endl;
        } else {
            program << tab << "add" << postfix << "    " << source << ", " << destination << std::endl;
        }
    }
    
    void sub(std::string source, std::string destination, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "sub     " << source << ", " << destination << std::endl;
        } else {
            program << tab << "sub" << postfix << "    " << source << ", " << destination << std::endl;
        }
    }
    
    void inc(std::string reg) {
        program << tab << "inc     " << reg << std::endl;
    }
    
    void mul(std::string source, std::string destination, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "imul    " << source << ", " << destination << std::endl;
        } else {
            program << tab << "imul" << postfix << "   " << source << ", " << destination << std::endl;
        }
    }
    
    void mul(std::string destination, std::string source, std::string constant, std::string postfix) {
        if (postfix == "") {
            program << tab << "imul    " << constant << ", " << source << ", " << destination << std::endl;
        } else {
            program << tab << "imul" << postfix << "   ";
            program << constant << ", " << source << ", " << destination << std::endl;
        }
    }
    
    // maybe pass in the value that will be the numerator and move into registers here
    void div(std::string destination) {
        program << tab << "idiv    " << destination << std::endl;
    }
    
    // compare
    void cmp(std::string left, std::string right, std::string postfix = "") {
        if (postfix == "") {
            program << tab << "cmp     " << left << ", " << right << std::endl;
        } else {
            program << tab << "cmp" << postfix << "    " << left << ", " << right << std::endl;
        }
    }
    
    // jump
    void jmp(std::string label) {
        program << tab << "jmp     " << label << std::endl;
    }
    
    void je(std::string label) {
        program << tab << "je      " << label << std::endl;
    }
    
    void jne(std::string label) {
        program << tab << "jne     " << label << std::endl;
    }
    
    void jg(std::string label) {
        program << tab << "jg      " << label << std::endl;
    }
    
    void jge(std::string label) {
        program << tab << "jge     " << label << std::endl;
    }
    
    void jl(std::string label) {
        program << tab << "jl      " << label << std::endl;
    }
    
    void jle(std::string label) {
        program << tab << "jle     " << label << std::endl;
    }
    
    // function call/return
    
    void call(std::string label) {
        program << tab << "call    " << label << std::endl;
    }
    
    void ret() {
        program << tab << "ret" << std::endl;
    }
    
    // set
    void sete(std::string label) {
        program << tab << "sete    " << label << std::endl;
    }
    
    void setne(std::string label) {
        program << tab << "setne   " << label << std::endl;
    }
    
    void setg(std::string label) {
        program << tab << "setg    " << label << std::endl;
    }
    
    void setge(std::string label) {
        program << tab << "setge   " << label << std::endl;
    }
    
    void setl(std::string label) {
        program << tab << "setl    " << label << std::endl;
    }
    
    void setle(std::string label) {
        program << tab << "setle   " << label << std::endl;
    }
    
    // formats
    void add_int_format() {
        if (!int_format_added) {
            formats << int_format << ":" << std::endl;
            formats << tab << ".asciz \"%d\\n\"";
            int_format_added = true;
        }
    }
    
};

#endif /* code_generator_h */
