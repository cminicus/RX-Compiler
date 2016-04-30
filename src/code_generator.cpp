//
//  code_generator.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/30/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "code_generator.h"

std::string CodeGenerator::generate() {
    program_setup();
    parse_symbol_table();
    return parse_ast();
}

void CodeGenerator::parse_symbol_table() {
    // look for global/static variables in the symbol table
    parse_scope(symbol_table.get_current_scope());
}

void CodeGenerator::parse_scope(Scope * scope) {
    int total_size = 0;
    
    for (auto it = scope->entries.begin(); it != scope->entries.end(); ++it) {
        Entry * entry = it->second;
        
        if (entry->get_entry_kind() == VARIABLE_ENTRY) {
            Variable * v = dynamic_cast<Variable *>(entry);
            int size = v->type->get_type_size();
            
            v->stack_offset = current_stack_offset;
            
            total_size += size;
            current_stack_offset += size;
            
        } else if (entry->get_entry_kind() == CONSTANT_ENTRY) {
            Constant * c = dynamic_cast<Constant *>(entry);
            int size = c->type->get_type_size();
            
            c->stack_offset = current_stack_offset;
            
            total_size += size;
            current_stack_offset += size;
        }
    }
    
    scope->size = total_size;
    if (scope->size != 0) {
        allocate_stack_space(total_size);
    }
    
    new_line();
}

void CodeGenerator::end_scope(Scope * scope) {
    if (scope->size != 0) {
        deallocate_stack_space(scope->size);
        current_stack_offset -= scope->size;
    }
}

std::string CodeGenerator::parse_ast() {
    Node * root = ast.get_root();
    generate_instruction(dynamic_cast<InstructionNode *>(root));
    
    move("$0", rax);
    function_teardown();
    
    new_line();
    program << formats.str();
    
    return program.str();
}

void CodeGenerator::program_setup() {
    add_directive(".text");
    add_directive(".globl", "_main");
    add_directive(".align", "4, 0x90");
    new_line();
    
    add_label("_main");
    function_setup();
}


// ------------------------------- Instructions --------------------------------

void CodeGenerator::generate_instruction(InstructionNode * node) {
    if (node == nullptr) {
        return;
    }
    
    switch (node->get_instruction_kind()) {
        case IF_INSTRUCTION:          generate_if         (dynamic_cast<IfNode *>(node)); break;
        case WHILE_INSTRUCTION:       generate_while      (dynamic_cast<WhileNode *>(node)); break;
        case ASSIGN_INSTRUCTION:      generate_assign     (dynamic_cast<AssignNode *>(node)); break;
        case PRINT_INSTRUCTION:       generate_print      (dynamic_cast<PrintNode *>(node)); break;
        case DECLARATION_INSTRUCTION: generate_declaration(dynamic_cast<DeclarationNode *>(node)); break;
    }
    
    generate_instruction(node->next);
}

void CodeGenerator::generate_if(IfNode * node) {
    
    int length = node->conditions.size();
    std::string end_label = next_label();
    
    // create array of labels
    std::vector<std::string> labels;
    for (int i = 0; i < length; i++) {
        labels.push_back(next_label());
    }
    
    // all if and else if's
    for (int i = 0; i < length; i++) {
        ConditionNode * condition_node = node->conditions.at(i);
        exp_packet condition = generate_condition(condition_node);
        
        if (condition.is_constant) {
            
            // always true
            if (condition.value) {
                
                generate_block(node->blocks.at(i)); // generate this branch
                jmp(end_label); // jump to end of statement
                new_line();
            }
            // go to next branch if false
            
        } else {
            
            pop(rax); // pop condition
            cmp(_constant(1), eax, "l"); // compare with true
            jne(labels.at(i)); // if not equal, skip this branch
            
            BlockNode * block = node->blocks.at(i);
            generate_block(block); // generate branch
            jmp(end_label); // jump to end if branch is executed
            
            add_label(labels.at(i));  // place label after instructions
            
            new_line();
        }
    }

    // else block, always generate this because any branch that executes will jump to end_label
    if (node->blocks.size() > node->conditions.size()) {
        BlockNode * else_block = node->blocks.at(node->blocks.size() - 1);
        generate_block(else_block);
    }
    
    add_label(end_label);
    
    new_line();
}

void CodeGenerator::generate_while(WhileNode * node) {
    std::string loop_label = next_label();
    std::string end_label = next_label();
    
    add_label(loop_label); // place loop_label
    
    exp_packet condition = generate_condition(node->condition); // generate condition
    
    if (condition.is_constant) {
        
        // always true
        if (condition.value) {
        
            generate_block(node->block);
            jmp(loop_label);
            new_line();
        }
        
        // do nothing if false
        
    } else {
        
        pop(rax); // pop condition
        cmp(_constant(1), eax, "l"); // compare with true
        jne(end_label); // if not equal, go to end
        
        generate_block(node->block);
        jmp(loop_label); // jump to the beginnging
        
        add_label(end_label); // place end label after instructions
        
        new_line();
    }
}

void CodeGenerator::generate_assign(AssignNode * node) {
    exp_packet offset = generate_location(node->location);
    exp_packet expression = generate_expression(node->expression);
    
    std::string reg = node->expression->type->get_type_kind() == INTEGER_TYPE ? eax : rax;
    std::string postfix = (reg == eax) ? "l" : "";
    
    if (offset.is_constant && expression.is_constant &&
        node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        
        // move value at constant location into intermediate register
        move(_rbp(expression.value), reg);
        // move value into constant destination location
        move(reg, _rbp(offset.value));
        
    } else if (offset.is_constant && expression.is_constant) {
        // move constant value into constant offset
        move(_constant(expression.value), _rbp(offset.value), postfix);
        
    } else {
        pop(rax); // non-constant expression
        
        // dereference if needed
        dereference_if_needed(reg, node->expression);
        
        move(reg, _rbp(offset.value)); // move expression into rdx (holds location)
    }

    new_line();
}

void CodeGenerator::generate_declaration(DeclarationNode * node) {
    exp_packet offset = generate_location(node->location);
    exp_packet expression = generate_expression(node->expression);
    
    assert(offset.is_constant); // location should be constant for declaration
    
    // primitive type and structure type (int/float/bool/char vs struct/class/string/array)
    // to determine whether to do this set of if/else if/else or the kind that copies full blocks of memory
    
    std::string reg = node->expression->type->get_type_kind() == INTEGER_TYPE ? eax : rax;
    std::string postfix = (reg == eax) ? "l" : "";
    
    if (expression.is_constant &&
        node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        
        // move value at constant location into intermediate register
        move(_rbp(expression.value), reg);
        // move value into constant destination location
        move(reg, _rbp(offset.value));
        
    } else if (expression.is_constant) {
        
        // move constant value into constant offset
        move(_constant(expression.value), _rbp(offset.value), postfix);
        
    } else {
        
        pop(rax); // non-constant expression
        
        // dereference if needed
        dereference_if_needed(reg, node->expression);
        
        move(reg, _rbp(offset.value)); // move expression into rdx (holds location)
    }
    
    new_line();
}

void CodeGenerator::generate_print(PrintNode * node) {
    exp_packet expression = generate_expression(node->expression);
    
    if (expression.is_constant && node->expression->get_expression_kind() == LOCATION_EXPRESSION) {
        // constant offset
        move(_rbp(expression.value), rsi);
        
    } else if (expression.is_constant) {
        // constant value
        move(_constant(expression.value), rsi);
        
    } else {
        // non-constant value
        pop(rsi);
        
        // dereference if needed
        dereference_if_needed(rsi, node->expression);
    }
    
//    parse_expression_packet(node->expression, expression, rsi);
    
    load(_rip(int_format), rdi);
    
    call(printf);
    new_line();
    
    add_int_format();
}

// -------------------------------- Conditions ---------------------------------

exp_packet CodeGenerator::generate_condition(ConditionNode * node) {
    exp_packet left = generate_expression(node->left);
    exp_packet right = generate_expression(node->right);
    
    // both are constant values (not offsets)
    if (left.is_constant && node->left->get_expression_kind() != LOCATION_EXPRESSION &&
        right.is_constant && node->right->get_expression_kind() != LOCATION_EXPRESSION) {
        
        switch (node->operation) {
            case EQUALS: return {true, left.value == right.value ? true : false};
            case NOT_EQUALS: return {true, left.value != right.value ? true : false};
            case LESS_THAN: return {true, left.value < right.value ? true : false};
            case LESS_THAN_EQUALS: return {true, left.value <= right.value ? true : false};
            case GREATER_THAN: return {true, left.value > right.value ? true : false};
            case GREATER_THAN_EQUALS: return {true, left.value >= right.value ? true : false};
            default: return {false};
        }
    }
    
    std::string right_reg = node->right->type->get_type_kind() == INTEGER_TYPE ? eax : rax;
    std::string left_reg = node->left->type->get_type_kind() == INTEGER_TYPE ? ebx : rbx;
    std::string postfix = node->left->type->get_type_kind() == INTEGER_TYPE ? "l" : ""; // see if we need postfix
    
    // move right expression into eax/rax
    parse_expression_to_register(right, node->right, right_reg, rax, postfix);
    
    // move left expression into ebx/rbx
    parse_expression_to_register(left, node->left, left_reg, rbx, postfix);
    
    cmp(right_reg, left_reg, postfix);
    
    // TODO: - push and set "%al% here and know you need to pop into that in the condition statements
    
    move(_constant(0), rax); // clear rax
    push(rax);
    
    switch (node->operation) {
        case EQUALS:              sete(_rsp_); break; // set top of stack if equal
        case NOT_EQUALS:          setne(_rsp_); break; // set top of stack if not equal
        case LESS_THAN:           setl(_rsp_); break; // set top of stack if less than
        case LESS_THAN_EQUALS:    setle(_rsp_); break; // set top of stack if less than or equal
        case GREATER_THAN:        setg(_rsp_); break; // set top of stack if greater than
        case GREATER_THAN_EQUALS: setge(_rsp_); break; // set top of stack if greater than or equal
        default: break;
    }
    
//    push(rax);
    new_line();
    
    return {false};
}

// ---------------------------------- Blocks -----------------------------------

void CodeGenerator::generate_block(BlockNode * node) {
    parse_scope(node->scope);
    generate_instruction(node->instructions);
    end_scope(node->scope);
    new_line();
}

// -------------------------------- Expresions ---------------------------------

exp_packet CodeGenerator::generate_expression(ExpressionNode * node) {
    switch (node->get_expression_kind()) {
        case NUMBER_EXPRESSION:   return generate_number  (dynamic_cast<NumberNode *>(node)); break;
        case BINARY_EXPRESSION:   return generate_binary  (dynamic_cast<BinaryNode *>(node)); break;
        case LOCATION_EXPRESSION: return generate_location(dynamic_cast<LocationNode *>(node)); break;
    }
}

exp_packet CodeGenerator::generate_number(NumberNode * node) {
    return {true, node->constant->value};
}

exp_packet CodeGenerator::generate_binary(BinaryNode * node) {
    exp_packet left = generate_expression(node->left);
    exp_packet right = generate_expression(node->right);
    
    // both are constant values (not offsets)
    if (left.is_constant && node->left->get_expression_kind() != LOCATION_EXPRESSION &&
        right.is_constant && node->right->get_expression_kind() != LOCATION_EXPRESSION) {
        
        switch (node->operation) {
            case PLUS:     return {true, left.value + right.value};
            case MINUS:    return {true, left.value - right.value};
            case MULTIPLY: return {true, left.value * right.value};
            case DIVIDE:   return {true, left.value / right.value};
            case MODULO:   return {true, left.value % right.value};
            default:       return {false};
        }
    }
    
    std::string right_reg = node->right->type->get_type_kind() == INTEGER_TYPE ? eax : rax;
    std::string left_reg = node->left->type->get_type_kind() == INTEGER_TYPE ? ebx : rbx;
    std::string postfix = node->type->get_type_kind() == INTEGER_TYPE ? "l" : ""; // see if we need postfix
    
    // move right expression into eax/rax
    parse_expression_to_register(right, node->right, right_reg, rax, postfix);
    
    // move left expression into ebx/rbx
    parse_expression_to_register(left, node->left, left_reg, rbx, postfix);

    switch (node->operation) {
        case PLUS:     add(right_reg, left_reg, postfix); break;
        case MINUS:    sub(right_reg, left_reg, postfix); break; // 2nd - 1st, store in 2nd
        case MULTIPLY: mul(right_reg, left_reg, postfix); break;
        case DIVIDE:
            // TODO: - divide
            break;
        case MODULO:
            // TODO: - modulo
            break;
        default:
            break;
    }
    
    push(rbx);
    new_line();
    
    return {false}; // value is optional
}

exp_packet CodeGenerator::generate_location(LocationNode * node) {
    switch (node->get_location_kind()) {
        case VARIABLE_LOCATION: return generate_variable(dynamic_cast<VariableNode *>(node)); break;
        case CONSTANT_LOCATION: return generate_constant(dynamic_cast<ConstantNode *>(node)); break;
    }
}

exp_packet CodeGenerator::generate_variable(VariableNode * node) {
    return {true, node->variable->stack_offset};
}

exp_packet CodeGenerator::generate_constant(ConstantNode * node) {
    return {true, node->constant->stack_offset};
}

void CodeGenerator::dereference_if_needed(std::string reg, ExpressionNode * expression) {
    if (expression->get_expression_kind() == LOCATION_EXPRESSION) {
        std::string deref = "(" + reg + ")";
        move(deref, reg); // dereference if needed
    }
}


void CodeGenerator::parse_expression_packet(ExpressionNode * node, exp_packet expression, std::string reg) {
    
    if (expression.is_constant && node->get_expression_kind() == LOCATION_EXPRESSION) {
        // constant offset
        move(_rbp(expression.value), reg);
        
    } else if (expression.is_constant) {
        // constant value
        move(_constant(expression.value), reg);
        
    } else {
        // non-constant value
        pop(reg);
        
        if (node->get_expression_kind() == LOCATION_EXPRESSION) {
            std::string dereference = "(" + reg + ")";
            move(reg, reg); // dereference if needed
        }
    }
}

void CodeGenerator::parse_expression_to_register(exp_packet packet,
                                                 ExpressionNode * node,
                                                 std::string reg,
                                                 std::string reg64,
                                                 std::string postfix) {
    
    if (packet.is_constant && node->get_expression_kind() == LOCATION_EXPRESSION) {
        
        move(_rbp(packet.value), reg, postfix); // move value at offset to BX
        
        // constant value
    } else if (packet.is_constant) {
        
        move(_constant(packet.value), reg, postfix); // move constant to BX
        
        // non-constant offset/value
    } else {
        
        pop(reg64); // pop left hand argument
        dereference_if_needed(reg, node); // dereference if needed
    }
}
