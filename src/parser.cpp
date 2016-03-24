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
void Parser::close_scope() {
    if (symbol_table != nullptr) {
        symbol_table->close_scope();
    }
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
Entry* Parser::find(std::string name) {
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
 */
void Parser::create_variable_entry(token_match match) {
    if (match.is_valid && !no_symbols) {
        Token t = match.token;
        // other things with local??
        if (local(t.identifier)) {
            // error
            // duplicate declaration
            Entry *e = find(t.identifier);
            // pass in entry and get e->line_pos and e->col_pos
            ErrorHandler::duplicate_identifier(false, t, e);
        } else {
            Variable *v = new Variable;
            v->value = 5;
            v->type = Integer::Instance();
            v->line_position = t.line_position;
            v->col_position = t.col_position;
            
            insert(t.identifier, v);
        }
    }
}

/**
 *  Creates a constant entry for the symbol table
 *
 *  @param match the token_match struct
 */
void Parser::create_constant_entry(token_match match) {
    if (match.is_valid && !no_symbols) {
        Token t = match.token;
        if (local(t.identifier)) {
            // error
            // duplicate declaration
            Entry *e = find(t.identifier);
            ErrorHandler::duplicate_identifier(false, t, e);
        } else {
            Constant *c = new Constant;
            c->value = 5;
            c->type = Integer::Instance();
            c->line_position = t.line_position;
            c->col_position = t.col_position;
            
            insert(t.identifier, c);
        }
    }
}

#pragma mark - Grammar
// ----------------------------- Grammar Functions -----------------------------

/**
 *  Statements = {Instructions}
 */
void Parser::Statements() {
    InstructionNode * root = nullptr;
    InstructionNode * current_node = nullptr;
    InstructionNode * next_node = nullptr;
    while (current.kind != END_OF_FILE) {
        
        // first thing has to be an instruction or the file is empty
        if (!check(this->instructions)) {
            // if we miss an instruction, sync until we do
            ErrorHandler::expected_instruction(false, current);
            sync(this->instructions);
        }
        
//        if (root == nullptr) {
//            root = Instructions();
//            current_node = root;
//        } else {
//            next_node = Instructions();
//            current_node->next = next_node;
//            current_node = next_node; // is this right?
//        }
        
        /* root->next = Instructions(); */
        Instructions();
    }
    
    // set root node for the ast
    set_ast_root(root);
    
    // only throws errors if any have been flagged
    ErrorHandler::throw_errors();
}

/**
 *  Declaration = VariableDeclaration | ConstantDeclaration
 */
void Parser::Declaration() {
    // right now you can correctly declare a variable called "int"
    // have a list of types potentially in the symbol_table and always
    // check against that in any declaration (even if not local)
    if (check({VAR})) {
        VariableDeclaration();
        
    } else if (check({LET})) {
        ConstantDeclaration();
    }
}


/**
 *  VariableDeclaration = "var" identifier "=" Expression
 */
void Parser::VariableDeclaration() {
    match(VAR);
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    Expression();
    
    create_variable_entry(m);
}

/**
 *  ConstantDeclaration = "let" identifier "=" Expression
 */
void Parser::ConstantDeclaration() {
    match(LET);
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    Expression();
    
    create_constant_entry(m);
}

/**
 *  Expression = [“+” | “-“] Term {(“+” | “-“) Term}
 */
ExpressionNode * Parser::Expression() {
    // probably isn't right
    optional_match({PLUS, MINUS});
    
    Term();
    
    while (optional_match({PLUS, MINUS})) {
        Term();
    }
    
    return nullptr;
}

/**
 *  Term = Factor {(“*” | “/“ | “%”) Factor}
 */
void Parser::Term() {
    Factor();
    while (optional_match({MULTIPLY, DIVIDE, MODULO})) {
        Factor();
    }
}

/**
 *  Factor = integer | identifier | “(“ Expression “)”
 */
ExpressionNode * Parser::Factor() {
    if (check({INTEGER})) {
        token_match m = match(INTEGER);
        
        NumberNode * number_node = new NumberNode;
        Constant * c = new Constant;
        c->type = Integer::Instance();
        c->value = m.token.value;
        number_node->constant = c;
        
        return number_node;
        
    } else if (check({IDENTIFIER})) {
        token_match m = match(IDENTIFIER);
        
        if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
            ErrorHandler::undeclared_identifier(false, m.token);
        }
        
        // do the whole variable thing here
        Entry * e = find(m.token.identifier);
        if (e == nullptr) {
            // TODO: throw error, cannot assign to non variable type
        }
        
        // cast entry to a variable and make a variable_node
        // this could also be a constant here! constant_node?
        Variable * variable = dynamic_cast<Variable *>(e);
        VariableNode * variable_node = new VariableNode;
        
        // attach variable to variable node
        variable_node->variable = variable;
        return variable_node;
        
    } else if (check({OPEN_PAREN})) {
        match(OPEN_PAREN);
        Expression();
        match(CLOSE_PAREN);
        
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
    // have to make these points unique every loop -> new pointer?
    InstructionNode * instructions = nullptr;
    InstructionNode * current = nullptr;
    InstructionNode * next = nullptr;
    
    while (check(this->instructions)) {
        
//        if (instructions == nullptr) {
//            instructions = Instruction();
//            current = instructions;
//        } else {
//            next = Instruction();
//            current->next = next;
//            current = next;
//        }
        
        Instruction();
        
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
        /*instruction_node = */Declaration();
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
    if_node->condition = Condition();
    
    match(OPEN_CURLY);
    optional_match({NEW_LINE});
    // create new scope for if statement
    create_scope();
    
    // have a vector of instructions for the if?
    // like not true, false
    // inst_1, inst_2, inst_3 etc...
    if_node->true_instructions = Instructions();
    
    optional_match({NEW_LINE});
    match(CLOSE_CURLY);
    
    // end scope for if statement
    close_scope();
    
elseif:
    // else or else if clause
    if (check({ELSE})) {
        match(ELSE);
        
        // else if clause
        if (optional_match({IF})) {
            Condition();
            
            match(OPEN_CURLY);
            optional_match({NEW_LINE});
            // create new scope for else if statement
            create_scope();
            
            Instructions();
            
            optional_match({NEW_LINE});
            match(CLOSE_CURLY);
            
            // end scope for else if statement
            close_scope();
            
            goto elseif;
            
        // else clause
        } else {
            match(OPEN_CURLY);
            optional_match({NEW_LINE});
            // create new scope for else statement
            create_scope();
            
            Instructions();
            
            optional_match({NEW_LINE});
            match(CLOSE_CURLY);
            
            // end scope for else statement
            close_scope();
        }
    }
    return if_node;
}

/**
 *  While = “while” Condition “{“ [Instructions] “}”
 */
WhileNode * Parser::While() {
    WhileNode *while_node = new WhileNode;
    
    match(WHILE);
    while_node->condition = Condition();
    
    match(OPEN_CURLY);
    optional_match({NEW_LINE});
    // create new scope for while statement
    create_scope();
    
    while_node->instructions = Instructions();
    
    optional_match({NEW_LINE});
    match(CLOSE_CURLY);
    
    // end scope for while statement
    close_scope();
    return while_node;
}

/**
 *  Assign = identifier “=“ Expression
 */
AssignNode * Parser::Assign() {
    AssignNode * assign_node = new AssignNode;
    
    token_match m = match(IDENTIFIER);
    match(ASSIGN);
    assign_node->expression = Expression();
    
    if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
        // error
        // use of undeclared identifier
        ErrorHandler::undeclared_identifier(false, m.token);
    }
    
    Entry * e = find(m.token.identifier);
    if (e == nullptr || e->get_entry_kind() != VARIABLE_ENTRY) {
        // TODO: throw error, cannot assign to non variable type
    }
    
    // cast entry to a variable and make a variable_node
    Variable * variable = dynamic_cast<Variable *>(e);
    VariableNode * variable_node = new VariableNode;
    
    // attach variable to variable node
    variable_node->variable = variable;
    
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
    token_match m = match(IDENTIFIER);
    match(CLOSE_PAREN);
    
    if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
        // error
        // use of undeclared identifier
        ErrorHandler::undeclared_identifier(false, m.token);
    }
    
    Entry * e = find(m.token.identifier);
    if (e == nullptr) {
        // you can actually print nullptr here most likely? idk
        // TODO: throw error, cannot assign to non variable type
    }
    
    // TODO: - factor this out into a function (create_variable(m.token))
    // cast entry to a variable and make a variable_node
    Variable * variable = dynamic_cast<Variable *>(e);
    VariableNode * variable_node = new VariableNode;
    
    // attach variable to variable node
    variable_node->variable = variable;
    
    // attach variable_node to assgin_node
    print_node->location = variable_node;
    
    return print_node;
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
