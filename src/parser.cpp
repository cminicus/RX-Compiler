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
    ExpressionNode * root = Statements();
    
    // set root node for the ast
    set_ast_root(root);
    
    // only throws errors if any have been flagged
    ErrorHandler::throw_errors();
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

int Parser::get_token_precedence(Token t) {
    // get precedence
    int precedence = BinaryPrecedence[Token::mapping[t.kind]];
    
    // if not found (0), return -1
    if (precedence <= 0) { return -1; }
    
    // otherwise return precedence
    return precedence;
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

# pragma mark - Typing

void Parser::check_binary_operator_typing(Token operation_token, ExpressionNode * left, ExpressionNode * right) {
    
    // type mismatch
    if (*(left->type) != *(right->type)) {
        ErrorHandler::operation_type_mismatch(false, operation_token, left->type, right->type);
    }
    
    // if this is a number operator and we are not using a number -> error
    if (std::find(BinaryNumberOps.begin(), BinaryNumberOps.end(), operation_token.kind) != BinaryNumberOps.end()
        && (left->type->get_type_kind() != INTEGER_TYPE || right->type->get_type_kind() != INTEGER_TYPE)) {
        
        ErrorHandler::operation_type_mismatch(false, operation_token, left->type, right->type);
    }
    
    // if this is a boolean operator and we are not using a boolean -> error
    if (std::find(BinaryBooleanOps.begin(), BinaryBooleanOps.end(), operation_token.kind)
        != BinaryBooleanOps.end() &&
        (left->type->get_type_kind() != BOOLEAN_TYPE || right->type->get_type_kind() != BOOLEAN_TYPE)) {
        
        ErrorHandler::operation_type_mismatch(false, operation_token, left->type, right->type);
    }
}

void Parser::check_unary_operator_typing(Token operation_token, ExpressionNode * expression) {
    
    // if this is a number operator and we are not using a number -> error
    if (std::find(UnaryNumberOps.begin(), UnaryNumberOps.end(), operation_token.kind) != UnaryNumberOps.end() &&
        (expression->type->get_type_kind() != INTEGER_TYPE)) {
        
        ErrorHandler::illegal_operation_for_type(false, operation_token, expression->type);
    }
    
    // if this is a boolean operator and we are not using a boolean -> error
    if (std::find(UnaryBooleanOps.begin(), UnaryBooleanOps.end(), operation_token.kind) != UnaryBooleanOps.end()
        && (expression->type->get_type_kind() != BOOLEAN_TYPE)) {
        
        ErrorHandler::illegal_operation_for_type(false, operation_token, expression->type);
    }
}

Type * Parser::get_type_annotation() {
    // check for type annotation
    if (optional_match({COLON})) {
        token_match m = match(IDENTIFIER);
        
        Entry * type = find(m.token.identifier);
        
        // undeclared identifier
        if (!no_symbols && type == nullptr) {
            ErrorHandler::undeclared_identifier(false, m.token);
        }
        
        if (type != nullptr && type->get_entry_kind() != TYPE_ENTRY) {
            ErrorHandler::identifier_must_be_type(false, m.token);
        }
        
        return dynamic_cast<Type *>(type);
    }
    
    return nullptr;
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
 *  Statements = {Statement {{";" | "\n"} Statement}}
 */
ExpressionNode * Parser::Statements() {
    
    ExpressionNode * root = nullptr;
    ExpressionNode * current_node = nullptr;
    
    while (current.kind != END_OF_FILE) {
        
        // statements can be ended be a closed curly because they are optional
        // if x == 5 {~statements~}
        if (check({CLOSE_CURLY})) {
            break;
        }
        
        if (no_ast) {
            Statement();
        } else {
            
            if (root == nullptr) {
                root = Statement();
                current_node = root;
                
//                if (current_node == nullptr) {
//                    continue;
//                }

            } else {
                current_node->next = Statement();
                if (current_node->next != nullptr) {
                    current_node = current_node->next;
                }
            }
        }
        
        if (check({CLOSE_CURLY, END_OF_FILE})) {
            break;
        }
        
        if (!optional_match(file_end)) {
            // error -> expected end of statement or end of file
            ErrorHandler::test_error(false);
            sync(statement_end);
        }
        
        while (optional_match(statement_end)); // eat all statement end tokens
        
        // first thing has to be an instruction or the file is empty
//        if (!check(this->instructions)) {
//            
//            // if we miss an instruction, sync until we do
//            ErrorHandler::expected_instruction(false, current);
//            sync(this->instructions);
//        }
//
//        if (no_ast) {
//            Instructions();
//            
//        } else {
//            if (root == nullptr) {
//                root = Instructions();
//                current_node = root;
//                
//                if (current_node == nullptr) {
//                    continue;
//                }
//                
//                // move current node to the end of the instruction list
//                while (current_node->next != nullptr) {
//                    current_node = current_node->next;
//                }
//                
//            } else {
//                current_node->next = Instructions();
//                
//                // move current node to the end of the instruction list
//                while (current_node->next != nullptr) {
//                    current_node = current_node->next;
//                }
//            }
//        }
    }
    
    return root;
}

/**
 *  Statement = {Declaration | Expression}
 */
ExpressionNode * Parser::Statement() {
    
    // declarations
    if (check({VAR, LET})) {
        return Declaration();
        
    // expressions
    } else {
        return Expression();
    }
}

/**
 *  Declaration = VariableDeclaration | ConstantDeclaration
 */
ExpressionNode * Parser::Declaration() {
    
    if (check({VAR})) {
        return VariableDeclaration();
        
    } else if (check({LET})) {
        return ConstantDeclaration();
    }

    return nullptr;
}


/**
 *  VariableDeclaration = "var" identifier [":" identifier] ["=" Expression]
 */
ExpressionNode * Parser::VariableDeclaration() {
    
    match(VAR);
    token_match m = match(IDENTIFIER);
    
    // check for type annotation
    Type * type_annotation = get_type_annotation();
    ExpressionNode * e = nullptr;
    
    // this means end of definition -> no expression
    Variable * v = nullptr;
    if (check(file_end)) { // only check, the actual parsing of this is handled in statements
        
        // error -> no expression without annotation
        if (!no_symbols && type_annotation == nullptr) {
            ErrorHandler::non_annotated_variable_must_have_expression(false, m.token);
        }
        
        v = create_variable_entry(m, type_annotation);
        
    // otherwise, there should be an expression
    } else {
        
        match(ASSIGN);
        e = Expression();
        
        // check type of annotation if any vs type of expression
        if (type_annotation != nullptr && *(type_annotation) != *(e->type)) {
            ErrorHandler::expression_and_annotation_mismatch(false, m.token, type_annotation, e->type);
        }
        
        v = create_variable_entry(m, e == nullptr ? nullptr : e->type);
    }
    
    if (!no_ast && v != nullptr) {
        
        VariableNode * variable = new VariableNode(v);
        return new BinaryNode(ASSIGN, variable, e); // operation, location, expression
    }
    
    return nullptr;
}

/**
 *  ConstantDeclaration = "let" identifier "=" Expression
 */
ExpressionNode * Parser::ConstantDeclaration() {
    match(LET);
    token_match m = match(IDENTIFIER);
    
    // check for type annotation
    Type * type_annotation = get_type_annotation();
    
    match(ASSIGN);
    ExpressionNode * e = Expression();
    
    // check type of annotation if any vs type of expression
    if (type_annotation != nullptr && *(type_annotation) != *(e->type)) {
        ErrorHandler::expression_and_annotation_mismatch(false, m.token, type_annotation, e->type);
    }
    
    Constant * c = create_constant_entry(m, e == nullptr ? nullptr : e->type);
    
    if (!no_ast && c != nullptr) {
        
        ConstantNode * constant = new ConstantNode(c);
        return new BinaryNode(ASSIGN, constant, e); // operation, location, expression
    }
    
    return nullptr;
}

/**
 *  Expression = Unary BinaryRHS
 */
ExpressionNode * Parser::Expression() {
    
    ExpressionNode * left = UnaryExpression();
    
    if (check({ASSIGN})) {
        return Assign(left);
    }
    
    if (check(BinaryOps)) {
        return BinaryExpression(0, left);
    }
    
    return left;
}

/**
 *  BinaryRHS = {BinaryOp Unary}
 */
ExpressionNode * Parser::BinaryExpression(int precedence, ExpressionNode * left) {
    
    while (check(BinaryOps)) {
        
        Token current_op = current;
        
        // get current precedence and see how it compares
        int current_precedence = get_token_precedence(current_op);
        if (current_precedence < precedence) { // <
            return left;
        }
        
        next(); // move to next token
        
        // get right hand side
        ExpressionNode * right = UnaryExpression();
        
        // return if error and we are using the AST
        if (!no_ast && right == nullptr) {
            return nullptr;
        }

        // check precedence of next token to see if it's another op
        Token next_op = current;
        int next_precedence = get_token_precedence(next_op);
        
        if (next_precedence >= 0 && current_precedence < next_precedence) { // <
            
            // if current_op binds less tightly with the right expression than the operator after
            // the right expression, let that operator take the right expression as it's left expression
            right = BinaryExpression(current_precedence + 1, right);
            
            if (!no_ast && right == nullptr) {
                return nullptr;
            }
        }
        
        // merge left and right into binary node
        if (!no_ast) {
            
            // check binary operator typing
            check_binary_operator_typing(current_op, left, right);
            
            left = new BinaryNode(current_op.kind, left, right);
            
            // if this is a RelOp, turn the node into a boolean type
            if (std::find(RelOps.begin(), RelOps.end(), current_op.kind) != RelOps.end()) {
                left->type = new Boolean;
            }
        }
    }
    
    // return the left argument
    return left;
}

/**
 *  Unary = Primary | UnaryOp Unary
 */
ExpressionNode * Parser::UnaryExpression() {
    
    if (!check(UnaryOps)) {
        return Primary();
    }
    
    Token operation_token = current;
    next();
    
    if (ExpressionNode * operand = UnaryExpression()) {
        
        // check typing
        check_unary_operator_typing(operation_token, operand);
        
        // return new node
        return new UnaryNode(operation_token.kind, operand);
    }
    
    return nullptr;
}

/**
 *  Primary = integer | boolean | identifier | "(" Expression ")" | Instruction
 */
ExpressionNode * Parser::Primary() {
    
    if (check({INTEGER})) {
        
        token_match m = match(INTEGER);
        
        // leave if no ast
        if (no_ast) {
            return nullptr;
        }
        
        Constant * c = new Constant(m.token, new Integer(32));
        NumberNode * number_node = new NumberNode(c);
        
        return number_node;
        
    } else if (check({TRUE_TOK, FALSE_TOK})) {
        
        token_match m = match({TRUE_TOK, FALSE_TOK});
        
        // leave if no ast
        if (no_ast) {
            return nullptr;
        }
        
        Constant * c = new Constant(m.token, new Boolean);
        BooleanNode * boolean_node = new BooleanNode(c);
        
        return boolean_node;
        
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
        
    // TODO: make this end of file and all special symbols d
    } else if (check({END_OF_FILE, EQUALS, OPEN_CURLY, CLOSE_PAREN})) {
        
        // shouldn't ever hit end of file here
        ErrorHandler::test_error(false);
        return nullptr;
        
    } else {
        // this will throw the incorrect match error for us
        // probably not the best error though
//        match({INTEGER, TRUE_TOK, FALSE_TOK, IDENTIFIER, OPEN_PAREN});
        
        // otherwise, the expression is an instruction
        return Instruction();
    }
    
    return nullptr;
}

/**
 *  Instructions = Instruction {{";" | "\\n"} Instruction}
 */
InstructionNode * Parser::Instructions() {
    
    InstructionNode * instructions = nullptr;
//    InstructionNode * current_instruction = nullptr;
//    
//    while (check(this->instructions)) {
//        
//        if (instructions == nullptr) {
//            instructions = Instruction();
//            current_instruction = instructions;
//        } else {
//            current_instruction->next = Instruction();
//            current_instruction = current_instruction->next;
//        }
//        
//        // ending file, or end of body is a reason to leave
//        if (check({END_OF_FILE, CLOSE_CURLY})) { // TODO: don't think you need curly here...
//            break;
//        }
//
//        while (optional_match({NEW_LINE, SEMI_COLON})); // TODO: this was changed to while optional_match, that okay??
//    }
    
    return instructions;
}

/**
 *  Instruction = If | While | Print | Scan
 */
InstructionNode * Parser::Instruction() {
    
    InstructionNode * instruction_node = nullptr;
    
    if (check({IF})) {
        instruction_node = If();
        
    } else if (check({WHILE})) {
        instruction_node = While();
        
    } else if (check({FOR})) {
        For();
//        instruction_node = For();
        
    } else if (check({PRINT})) {
        instruction_node = Print();
        
    } else if (check({SCAN})) {
        instruction_node = Scan();
        
    } else {
        // this is handled outside of here
//        ErrorHandler::expected_instruction(false, current);
//        sync(this->instructions);
    }
    
    return instruction_node;
}

/**
 *  Block = "{" [Statements] "}"
 */
BlockNode * Parser::Block() {
    BlockNode * block = new BlockNode;
    
    match(OPEN_CURLY);
    optional_match({NEW_LINE});
    
    // create new scope for while statement
    create_scope();
    
    block->expressions = Statements();
    
    optional_match({NEW_LINE});
    match(CLOSE_CURLY);
    
    block->scope = close_scope();
    
    return block;
}

/**
 *  If = "if" Expression Block {"else" "if" Expression Block} ["else" Block]
 */
IfNode * Parser::If() {
    IfNode * if_node = new IfNode;
    
    match(IF);
    
    Token position = current;
    ExpressionNode * e = Expression();
    
    if (e != nullptr && e->type->get_type_kind() != BOOLEAN_TYPE) {
        ErrorHandler::control_expression_not_boolean(false, position);
    }
    
    if_node->conditions.push_back(e);
    
    
    BlockNode * block = Block();
    if_node->blocks.push_back(block);
    
elseif:
    // else or else if clause
    if (optional_match({ELSE})) {
        
        // else if clause
        if (optional_match({IF})) {
            
            Token position = current;
            ExpressionNode * e = Expression();
            
            if (e != nullptr && e->type->get_type_kind() != BOOLEAN_TYPE) {
                ErrorHandler::control_expression_not_boolean(false, position);
            }
            
            if_node->conditions.push_back(e);
            
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
 *  While = "while" Expression Block
 */
WhileNode * Parser::While() {
    WhileNode * while_node = new WhileNode;
    
    match(WHILE);
    
    Token position = current;
    while_node->condition = Expression();
    
    if (while_node->condition != nullptr && while_node->condition->type->get_type_kind() != BOOLEAN_TYPE) {
        ErrorHandler::control_expression_not_boolean(false, position);
    }
    
    while_node->block = Block();
    
    if (no_ast) {
        delete while_node;
        return nullptr;
    }
    
    return while_node;
}

/**
 *  For = "for" [Instruction] ";" [Expression] ";" [Instruction] Block
 */
void Parser::For() {
    match(FOR);
    
    // initializer instruction list
    if (!check({SEMI_COLON})) {
        
        ExpressionNode * e = Statement(); // could be a declaration, eventually change to exp, varDecl, constDecl
        // else if (no_ast) { ErrorHandler::test_error(false); }
        if (BinaryNode * binary = dynamic_cast<BinaryNode *>(e)) {
            // binary node must be assign
            if (binary->operation != ASSIGN) {
                ErrorHandler::test_error(false);
            }
            
        } else if (!no_ast) {
            // must be an assignment node (binary)
            ErrorHandler::test_error(false);
        }

        // check for var/let and if so -> declaration instruciton,
        // otherwise expression (where x = 1 is an expression (assignment should be binary operator)
    }
    
    match(SEMI_COLON);
    
    // condition expression list
    if (!check({SEMI_COLON})) {
        ExpressionNode * e = Expression();
        
        // must be a boolean literal/expression
        if (e != nullptr && e->type->get_type_kind() != BOOLEAN_TYPE) {
            
            ErrorHandler::test_error(false);
        }
    }
    
    match(SEMI_COLON);
    
    // increment instruction list
    if (!check({OPEN_CURLY})) {
        Expression(); // this is an expression (assignment is a binary operation)
    }
    
    Block();
}

/**
 *  Assign = identifier "=" Expression
 */
ExpressionNode * Parser::Assign(ExpressionNode * left) {
    
//    token_match m = match(IDENTIFIER);
    Token t = last;
    match(ASSIGN);
    
    ExpressionNode * expression = Expression();
    
    VariableNode * variable = dynamic_cast<VariableNode *>(left);
    if (!no_ast && variable == nullptr) {
        ErrorHandler::non_variable_assignment(false, t);
        return nullptr;
    }
    
    
//    if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
//        ErrorHandler::undeclared_identifier(false, m.token);
//        // maybe return here
//    }
    
//    Entry * e = find(m.token.identifier);
    
    // leave if not using ast
    if (no_ast) {
        return nullptr;
    }
    
//    if (e == nullptr || e->get_entry_kind() != VARIABLE_ENTRY) {
//        ErrorHandler::non_variable_assignment(false, m.token);
//        return nullptr;
//    }
    
//    AssignNode * assign_node = new AssignNode;
//    assign_node->expression = expression;
//    
//    // cast entry to a variable and make a variable_node
//    Variable * variable = dynamic_cast<Variable *>(e);
//    VariableNode * variable_node = new VariableNode(variable);
    
    // make sure types match
//    if (*(variable_node->type) != *(assign_node->expression->type)) {
//        ErrorHandler::incompatible_assignment(false, m.token, assign_node->expression->type);
//    }
    
    if (*(left->type) != *(expression->type)) {
        ErrorHandler::incompatible_assignment(false, last, expression->type);
    }
    
    // attach variable_node to assgin_node
//    assign_node->location = variable_node;
    return new BinaryNode(ASSIGN, left, expression);
//    return assign_node;
}

/**
 *  Print = "print" "(" Expression ")"
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


/**
 *  Scan = "scan" "(" identifier ")"
 */
ScanNode * Parser::Scan() {
    
    match(SCAN);
    match(OPEN_PAREN);
    token_match m = match(IDENTIFIER);
    match(CLOSE_PAREN);
    
    if (no_symbols) {
        return nullptr;
    }
    
    if (!no_symbols && m.is_valid && !find(m.token.identifier)) {
        ErrorHandler::undeclared_identifier(false, m.token);
    }
    
    Entry * e = find(m.token.identifier);
    
    if (e == nullptr || e->get_entry_kind() != VARIABLE_ENTRY) {
        ErrorHandler::non_variable_assignment(false, m.token); // use this error for now ,it's fine
        // TODO Error for using a constant with scan
        return nullptr;
    }
    
    // leave if not using ast
    if (no_ast) {
        return nullptr;
    }

    ScanNode * scan_node = new ScanNode;
    
    // cast entry to a variable and make a variable_node
    Variable * variable = dynamic_cast<Variable *>(e);
    VariableNode * variable_node = new VariableNode(variable);
    
    // attach variable_node to assign_node
    scan_node->location = variable_node;
    
    return scan_node;
}
