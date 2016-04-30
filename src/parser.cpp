//
//  parser.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include "error_handler.h"
#include "parser.h"

#pragma mark - Initialization

/**
 *  Initializes a parser using a scanner for tokens
 *
 *  @param scanner the scanner initialized with the program
 *
 *  @return a parser object
 */
Parser::Parser(Scanner &scanner) : scanner(scanner) {
    no_symbols = true;
    no_ast = true;
    symbol_table = nullptr;
    ast = nullptr;
    next();
}

/**
 *  Initializes a parser using a scanner for tokens and a symbol table
 *
 *  @param scanner      the scanner initialized with the program
 *  @param symbol_table the symbol table to hold declarations
 *
 *  @return a parser object
 */
Parser::Parser(Scanner &scanner, SymbolTable *symbol_table) :
    scanner(scanner), symbol_table(symbol_table) {
        
    no_ast = true;
    ast = nullptr;
    next();
}

/**
 *  Initializes a parser using a scanner for tokens and a symbol table
 *
 *  @param scanner      the scanner initialized with the program
 *  @param symbol_table the symbol table to hold declarations
 *  @param ast          the abstract syntax tree to hold the tree repesentation
 *                      of the program
 *
 *  @return a parser object
 */
Parser::Parser(Scanner &scanner, SymbolTable *symbol_table, AST *ast) :
    scanner(scanner), symbol_table(symbol_table), ast(ast) {
    
    next();
}

/**
 *  Begins parsing by calling the first function of the grammar
 */
void Parser::parse() {
    Statements();
}

#pragma mark - Utility
// --------------------------- Utility Functions -------------------------------

/**
 *  Gets the next token from the scanner and saves the last token
 */
void Parser::next() {
    last = current;
    current = scanner.next();
}

/**
 *  Trys to match the current token with the input token_kind. If there is no
 *  match, an error is flagged
 *
 *  @param match the token_kind to match with
 *
 *  @return the encountered token, or a null token if not matched
 */
token_match Parser::match(token_kind match) {
    
    if (current.kind == match) {
        next();
        return {last, true};
    }
    
    // throw an error
    ErrorHandler::unexpected_token(false, current, match);
    
    // return invalid token
    Token t;
    return {t, false};
}

/**
 *  Attemps to match the current token with several input token_kinds. If there
 *  are no matches, an error is flagged
 *
 *  @param matches the vector of token_kinds to match with
 *
 *  @return the encountered token, or a null token if not matched
 */
token_match Parser::match(std::vector<token_kind> matches) {
    for (int i = 0; i < (int) matches.size(); i++) {
        if (current.kind == matches[i]) {
            next();
            return {last, true};
        }
    }
    
    // throw an error
    ErrorHandler::unexpected_token(false, current, matches);
    
    // return invalid token
    Token t;
    return {t, false};
}

/**
 *  Attemps to match current token with several input token_kinds. If a match
 *  is found, Parser::next() is called;
 *
 *  @param matches the vector of token_kinds to match with
 *
 *  @return true if matched, false otherwise
 */
bool Parser::optional_match(std::vector<token_kind> matches) {
    if (check(matches)) {
        next();
        return true;
    }
    return false;
}

/**
 *  Checks if the current token is a match with any of the input token_kinds
 *
 *  @param matches the token_kinds to match against
 *
 *  @return true if matched, false otherwise
 */
bool Parser::check(std::vector<token_kind> matches) {
    for (int i = 0; i < (int) matches.size(); i++) {
        if (current.kind == matches[i]) {
            return true;
        }
    }
    return false;
}

/**
 *  Syncs the parser with a set of given tokens
 *
 *  @param tokens the tokens to match with
 */
void Parser::sync(std::vector<token_kind> tokens) {
    while (!check(tokens) && current.kind != END_OF_FILE) {
        next();
    }
}

#pragma mark - Symbol Table
// --------------------------- Symbol Table Functions --------------------------

/**
 *  Creates a new scope in the symbol table if it exists
 */
void Parser::create_scope() {
    if (symbol_table != nullptr) {
        symbol_table->create_scope();
    }
}

/**
 *  Closes the current scope in the symbol table if it exists
 */
Scope * Parser::close_scope() {
    if (symbol_table != nullptr) {
        return symbol_table->close_scope();
    }
    
    return nullptr;
}

/**
 *  Inserts a declaration into the symbol table if it exists
 *
 *  @param name  the key for the entry
 *  @param entry the entry itself
 */
void Parser::insert(std::string name, Entry *entry) {
    if (symbol_table != nullptr) {
        symbol_table->insert(name, entry);
    }
}

/**
 *  Finds an entry in the symbol table if it exists
 *
 *  @param name the key for the entry
 *
 *  @return the entry if found, nullptr otherwise
 */
Entry * Parser::find(std::string name) {
    if (symbol_table != nullptr) {
        return symbol_table->find(name);
    }
    return nullptr;
}

/**
 *  Checks if a key is in the symbol table's current scope if it exists
 *
 *  @param name the key for the entry
 *
 *  @return true if in the current scope, false otherwise
 */
bool Parser::local(std::string name) {
    if (symbol_table != nullptr) {
        return symbol_table->local(name);
    }
    return false;
}

#pragma mark - AST
// ------------------------------- AST Functions -------------------------------

/**
 *  Sets the root of the AST if it exists
 *
 *  @param root the root node
 */
void Parser::set_ast_root(Node *root) {
    if (ast != nullptr) {
        ast->set_root(root);
    }
}

#pragma mark - Entry
// ------------------------------ Entry Functions ------------------------------

/**
 *  Creates a variable entry for the symbol table
 *
 *  @param match the token_match struct
 *  @param type the variable type
 *
 *  @return a variable node, or nullptr if it's a duplicate
 */
Variable * Parser::create_variable_entry(token_match match, Type * type) {
    if (match.is_valid && !no_symbols) {
        Token t = match.token;
        
        // other things with local??
        if (local(t.identifier)) {
            
            // duplicate declaration
            Entry * e = find(t.identifier);
            ErrorHandler::duplicate_identifier(false, t, e);
            
        } else {
            
            Variable * v = new Variable(t, type);
            insert(t.identifier, v);
            
            return v;
        }
    }
    return nullptr;
}

/**
 *  Creates a constant entry for the symbol table
 *
 *  @param match the token_match struct
 *  @param type the variable type
 *
 *  @return a constant node, or nullptr if it's a duplicate
 */
Constant * Parser::create_constant_entry(token_match match, Type * type) {
    if (match.is_valid && !no_symbols) {
        Token t = match.token;
        
        if (local(t.identifier)) {
            
            // duplicate declaration
            Entry * e = find(t.identifier);
            ErrorHandler::duplicate_identifier(false, t, e);
            
        } else {
            
            Constant * c = new Constant(t, type);
            insert(t.identifier, c);
            
            return c;
        }
    }
    return nullptr;
}

#pragma mark - Grammar
// ----------------------------- Grammar Functions -----------------------------

/**
 *  Statements = {Instructions}
 */
void Parser::Statements() {
    
    InstructionNode * root = nullptr;
    InstructionNode * current_node = nullptr;
    
    while (current.kind != END_OF_FILE) {
        
        // first thing has to be an instruction or the file is empty
        if (!check(this->instructions)) {
            
            // if we miss an instruction, sync until we do
            ErrorHandler::expected_instruction(false, current);
            sync(this->instructions);
        }
        
        if (no_ast) {
            Instructions();
            
        } else {
            if (root == nullptr) {
                root = Instructions();
                current_node = root;
                
                // move current node to the end of the instruction list
                while (current_node->next != nullptr) {
                    current_node = current_node->next;
                }
                
            } else {
                current_node->next = Instructions();
                
                // move current node to the end of the instruction list
                while (current_node->next != nullptr) {
                    current_node = current_node->next;
                }
            }
        }
    }
    
    // set root node for the ast
    set_ast_root(root);
    
    // only throws errors if any have been flagged
    ErrorHandler::throw_errors();
}

/**
 *  Declaration = VariableDeclaration | ConstantDeclaration
 */
DeclarationNode * Parser::Declaration() {
    // right now you can correctly declare a variable called "int"
    // have a list of types potentially in the symbol_table and always
    // check against that in any declaration (even if not local)
    
    DeclarationNode * node = nullptr;
    
    if (check({VAR})) {
        node = VariableDeclaration();
        
    } else if (check({LET})) {
        node = ConstantDeclaration();
    }
    
    return node;
}


/**
 *  VariableDeclaration = "var" identifier "=" Expression
 */
DeclarationNode * Parser::VariableDeclaration() {
    match(VAR);
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    ExpressionNode * e = Expression();
    
    Variable * v = create_variable_entry(m, e == nullptr ? nullptr : e->type);
    
    if (!no_ast && v != nullptr) {
        
        VariableNode * variable = new VariableNode(v);
        DeclarationNode * declaration = new DeclarationNode(variable, e); // location, expression
        
        return declaration;
    }
    
    return nullptr;
}

/**
 *  ConstantDeclaration = "let" identifier "=" Expression
 */
DeclarationNode * Parser::ConstantDeclaration() {
    match(LET);
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    ExpressionNode * e = Expression();
    
    Constant *c = create_constant_entry(m, e == nullptr ? nullptr : e->type);
    
    if (!no_ast && c != nullptr) {
        
        ConstantNode * constant = new ConstantNode(c);
        DeclarationNode * declaration = new DeclarationNode(constant, e);  // location, expression
        
        return declaration;
    }
    
    return nullptr;
}

/**
 *  Expression = [“+” | “-“] Term {(“+” | “-“) Term}
 */
ExpressionNode * Parser::Expression() {
    // not that there is currently no numeric collapsing in the expression
    
    // probably isn't right
    optional_match({PLUS, MINUS});
    
    Token operation_token = last;
    bool negate = operation_token.kind == MINUS;
    
    ExpressionNode * root = Term();
    
    bool illegal_type = !no_ast && root != nullptr && root->type->get_type_kind() != INTEGER_TYPE;

    // negate the first term
    if (!no_ast && negate) {
        Constant * c = new Constant;
        c->value = 0;
        
        NumberNode * zero = new NumberNode(c);
        BinaryNode * binary = new BinaryNode(last.kind, zero, root); // operation, left, right
        
        root = binary;
    }
    
    while (optional_match({PLUS, MINUS})) {
        Token operation_token = last;
        
        if (illegal_type) {
            ErrorHandler::illegal_operation_for_type(false, operation_token, root->type);
            illegal_type = false;
        }
        
        ExpressionNode * term = Term();
        
        if (!no_ast) {
            if (term->type->get_type_kind() != INTEGER_TYPE) {
                ErrorHandler::illegal_operation_for_type(false, operation_token, term->type);
            }
            
            if (term->type != root->type) {
                ErrorHandler::operation_type_mismatch(false, operation_token, term->type, root->type);
            }
            
            BinaryNode * binary = new BinaryNode(operation_token.kind, root, term); // operation, left, right
            
            root = binary;
        }
    }
    
    return root;
}

/**
 *  Term = Factor {(“*” | “/“ | “%”) Factor}
 */
ExpressionNode * Parser::Term() {
    
    ExpressionNode * root = Factor();
    
    // we flag an error but don't know the specific operation yet
    bool illegal_type = !no_ast && root != nullptr && root->type->get_type_kind() != INTEGER_TYPE;
    
    while (optional_match({MULTIPLY, DIVIDE, MODULO})) {
        Token operation_token = last;
        
        if (illegal_type) {
            ErrorHandler::illegal_operation_for_type(false, operation_token, root->type);
            illegal_type = false;
        }
        
        ExpressionNode * factor = Factor();
        
        if (!no_ast) {
            if (factor->type->get_type_kind() != INTEGER_TYPE) {
                ErrorHandler::illegal_operation_for_type(false, operation_token, factor->type);
            }
            
            if (factor->type != root->type) {
                ErrorHandler::operation_type_mismatch(false, operation_token, factor->type, root->type);
            }
            
            BinaryNode * binary = new BinaryNode(operation_token.kind, root, factor); // operation, left, right;
            
            root = binary;
        }
    }

    return root;
}

/**
 *  Factor = integer | identifier | “(“ Expression “)”
 */
ExpressionNode * Parser::Factor() {
    
    if (check({INTEGER})) {
        
        token_match m = match(INTEGER);
        
        // leave if no ast
        if (no_ast) {
            return nullptr;
        }
        
        Constant * c = new Constant(m.token, Integer::Instance());
        NumberNode * number_node = new NumberNode(c);
        
        return number_node;
        
    } else if (check({IDENTIFIER})) {
        
        token_match m = match(IDENTIFIER);
        
        if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
            ErrorHandler::undeclared_identifier(false, m.token);
        }
        
        // leave if no ast
        if (no_ast) {
            return nullptr;
        }
        
        // find the entry
        Entry * e = find(m.token.identifier);
        
        if (e != nullptr && e->get_entry_kind() == VARIABLE_ENTRY) {
            
            Variable * variable = dynamic_cast<Variable *>(e);
            VariableNode * variable_node = new VariableNode(variable);
            
            return variable_node;
            
        } else if (e != nullptr && e->get_entry_kind() == CONSTANT_ENTRY) {
            
            Constant * constant = dynamic_cast<Constant *>(e);
            ConstantNode * constant_node = new ConstantNode(constant);
            
            return constant_node;
        }
        
        return nullptr;
        
    } else if (check({OPEN_PAREN})) {
        
        match(OPEN_PAREN);
        ExpressionNode * e = Expression();
        match(CLOSE_PAREN);
        
        return e;
        
    } else {
        // this will throw the incorrect match error for us
        // probably not the best error though
        match({INTEGER, IDENTIFIER, OPEN_PAREN});
    }
    
    return nullptr;
}

/**
 *  Instructions = Instruction {{“;” | “\\n”} Instruction}
 */
InstructionNode * Parser::Instructions() {
    
    InstructionNode * instructions = nullptr;
    InstructionNode * current_instruction = nullptr;
    
    while (check(this->instructions)) {
        
        if (instructions == nullptr) {
            instructions = Instruction();
            current_instruction = instructions;
        } else {
            current_instruction->next = Instructions();
            current_instruction = current_instruction->next;
        }
        
        // ending file, or end of body is a reason to leave
        if (check({END_OF_FILE, CLOSE_CURLY})) {
            break;
        }

        // right here coming off this :if 2 < 4 { let x = 4 } let x = 6
        // the close curly is matched in "If()"
        // and it comes out here expecting a new_line or semi_colon
        // but there is a "let" -> if close_curly beforehand then maybe
        
        match({NEW_LINE, SEMI_COLON});
    }
    return instructions;
}

/**
 *  Instruction = If | While | Assign | Print | Declaration
 */
InstructionNode * Parser::Instruction() {
    
    InstructionNode * instruction_node = nullptr;
    
    if (check({IF})) {
        instruction_node = If();
        
    } else if (check({WHILE})) {
        instruction_node = While();
        
    } else if (check({IDENTIFIER})) {
        instruction_node = Assign();
        
    } else if (check({PRINT})) {
        instruction_node = Print();
        
    } else if (check({VAR, LET})) {
        instruction_node = Declaration();
        
    } else {
        // this is handled outside of here
        ErrorHandler::expected_instruction(false, current);
        sync(this->instructions);
    }
    
    return instruction_node;
}

/**
 *  If = “if” Condition “{“ [Instructions] “}” {“else” “if” Condition “{“ [Instructions] “}”}
 *  [“else” “{“ [Instructions] “}”]
 */
IfNode * Parser::If() {
    IfNode * if_node = new IfNode;
    
    match(IF);
    
    ConditionNode * c = Condition();
    if_node->conditions.push_back(c);
    
    
    BlockNode * block = Block();
    if_node->blocks.push_back(block);
    
elseif:
    // else or else if clause
    if (optional_match({ELSE})) {
        
        // else if clause
        if (optional_match({IF})) {
            
            ConditionNode * c = Condition();
            if_node->conditions.push_back(c);
            
            BlockNode * block = Block();
            if_node->blocks.push_back(block);

            goto elseif;
            
        // else clause
        } else {
            
            BlockNode * block = Block();
            if_node->blocks.push_back(block);
        }
    }
    
    if (no_ast) {
        delete if_node;
        return nullptr;
    }
    
    return if_node;
}

/**
 *  While = “while” Condition “{“ [Instructions] “}”
 */
WhileNode * Parser::While() {
    WhileNode * while_node = new WhileNode;
    
    match(WHILE);
    while_node->condition = Condition();
    while_node->block = Block();
    
    if (no_ast) {
        delete while_node;
        return nullptr;
    }
    
    return while_node;
}

/**
 *  Assign = identifier “=“ Expression
 */
AssignNode * Parser::Assign() {
    
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    ExpressionNode * expression = Expression();
    
    if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
        ErrorHandler::undeclared_identifier(false, m.token);
        // maybe return here
    }
    
    Entry * e = find(m.token.identifier);
    
    // leave if not using ast
    if (no_ast) {
        return nullptr;
    }
    
    if (e == nullptr || e->get_entry_kind() != VARIABLE_ENTRY) {
        ErrorHandler::non_variable_assignment(false, m.token);
        return nullptr;
    }
    
    AssignNode * assign_node = new AssignNode;
    assign_node->expression = expression;
    
    // cast entry to a variable and make a variable_node
    Variable * variable = dynamic_cast<Variable *>(e);
    VariableNode * variable_node = new VariableNode(variable);
    
    // make sure types match
    if (variable_node->type != assign_node->expression->type) {
        ErrorHandler::incompatible_assignment(false, m.token, assign_node->expression->type);
    }
    
    // attach variable_node to assgin_node
    assign_node->location = variable_node;
    
    return assign_node;
}

/**
 *  Print = “print” “(“ identifier “)”
 */
PrintNode * Parser::Print() {
    
    PrintNode * print_node = new PrintNode;
    
    match(PRINT);
    match(OPEN_PAREN);
    print_node->expression = Expression();
    match(CLOSE_PAREN);
    
    if (no_ast) {
        delete print_node;
        return nullptr;
    }
    
    return print_node;
}

BlockNode * Parser::Block() {
    BlockNode * block = new BlockNode;
    
    match(OPEN_CURLY);
    optional_match({NEW_LINE});
    
    // create new scope for while statement
    create_scope();
    
    block->instructions = Instructions();
    
    optional_match({NEW_LINE});
    match(CLOSE_CURLY);
    
    block->scope = close_scope();
    
    return block;
}

/**
 *  Condition = Expression (“==“ | “!=“ | “<“ | “<=“ | “>” | “>=“) Expression
 */
ConditionNode * Parser::Condition() {
    
    ConditionNode *condition = new ConditionNode;
    condition->left = Expression();
    // maybe change into checks and if not, send a custom error "missing condition"
    condition->operation = match(this->conditions).token.kind;
    condition->right = Expression();
    
    return condition;
}
