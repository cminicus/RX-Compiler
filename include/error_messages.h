//
//  error_messages.h
//  RX Compiler
//
//  Created by Clayton Minicus on 3/3/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef error_messages_h
#define error_messages_h

using std::to_string;

#define UNTERMINATED_COMMENT(line, col) "unterminated comment starting at " + to_string(line) + ":" + to_string(col)

#define ILLEGAL_CHARACTER(c, line, col) "illegal character '" + c + "' at " + to_string(line) + ":" + to_string(col)



#define EXPECTED_TOKEN(kind, enc, exp, line, col) "encountered " + kind + " '" + enc + "' at " + to_string(line) + ":" + to_string(col) + " but expected token '" + exp + "'"

#define EXPECTED_IDENTIFIER(kind, enc, line, col) "encountered " + kind + " '" + enc + "' at " + to_string(line) + ":" + to_string(col) + " but expected an identifier"

#define EXPECTED_INTEGER(kind, enc, line, col) "encountered " + kind + " '" + enc + "' at " + to_string(line) + ":" + to_string(col) + " but expected an integer"

// of the form _, _, _, or _
#define EXPECTED_MULTIPLE(kind, enc, exp, line, col) "encountered " + kind + " '" + enc + "' at " + to_string(line) + ":" + to_string(col) + " but expected " + exp



#define EXPECTED_INSTRUCTION(kind, enc, line, col) "encountered " + kind + " '" + enc + "' at " + to_string(line) + ":" + to_string(col) + " but expected an instruction"


#define DUPLICATE_IDENTIFIER(id, line, col, line2, col2) "duplicate identifier '" + id + "' at " + to_string(line) + ":" + to_string(col) + " conflicts with definition at " + to_string(line2) + ":" + to_string(col2)

#define UNDECLARED_IDENTIFIER(id, line, col) "undeclared identifier '" + id + "' at " + to_string(line) + ":" + to_string(col)


#define NON_VARIABLE_ASSIGNMENT(id, line, col) "cannot assign to non-variable identifier '" + id + "' at " + to_string(line) + ":" + to_string(col)

#define ILLEGAL_OPERATION_FOR_TYPE(op, type, line, col) "cannot apply operation '" + op + "' at " + to_string(line) + ":" + to_string(col) + " with expressions of type '" + type + "'"

#define OPERATION_TYPE_MISMATCH(op, type1, type2, line, col) "cannot apply operation '" + op + "' at " + to_string(line) + ":" + to_string(col) + " with expressions of type '" + type1 + "' and '" + type2 + "'"

#define INCOMPATIBLE_ASSIGNMENT(id, type, line, col) "cannot assign to variable '" + id + "' at " + to_string(line) + ":" + to_string(col) + " with incompatible type '" + type + "'"

#endif /* error_messages_h */
