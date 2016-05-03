//
//  llvm_generator_test.cpp
//  RX Compiler
//
//  Created by Clayton Minicus on 4/22/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#include <stdio.h>
#include <sstream>
#include <cstdlib>

#include "catch.hpp"
#include "rx_compiler.h"

void code_generator_test_helper(std::string program, std::string output) {
    // generate code
    RXCompiler compiler(BUILD);
    compiler.set_source(program);
    
    compiler.compile();
    
    // redirect output to compiler_output.txt
    std::system("./a.out > compiler_output.txt");
    
    // read in compiler_output.txt
    std::ifstream input("compiler_output.txt");
    std::string file((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    
    // check that generated output matches expected output
    REQUIRE(file == output);
    
    // remove left over files
    std::system("rm a.out && rm compiler_output.txt");
}

#pragma mark Print Blocks

TEST_CASE("print blocks generate correct code") {
    
    SECTION("print generates constant values correctly") {
        std::string program = "print(4)";
        code_generator_test_helper(program, "4\n");
    }
    
    SECTION("print generates constant-offset variables correctly") {
        std::string program = "var y = 5 \n print(y)";
        code_generator_test_helper(program, "5\n");
    }
    
    SECTION("print generates constant-offset constants correctly") {
        std::string program = "let y = 6 \n print(y)";
        code_generator_test_helper(program, "6\n");
    }
}

#pragma mark Binary Blocks

TEST_CASE("binary blocks generate correct code") {
    
    // Addition
    
    SECTION("constant value additions generates correctly") {
        std::string program = "print(4 + 12)";
        code_generator_test_helper(program, "16\n");
    }
    
    SECTION("left constant location additions generates correctly") {
        std::string program = "let x = 4; print(x + 12)";
        code_generator_test_helper(program, "16\n");
    }
    
    SECTION("right constant location additions generates correctly") {
        std::string program = "let x = 4; print(12 + x)";
        code_generator_test_helper(program, "16\n");
    }
    
    SECTION("all constant location additions generates correctly") {
        std::string program = "let x = 4; let y = 12; print(x + y)";
        code_generator_test_helper(program, "16\n");
    }
    
    // eventually non-constant variable (a[i])

    // Subtraction
    
    SECTION("constant value subtractions generates correctly") {
        std::string program = "print(4 - 12)";
        code_generator_test_helper(program, "-8\n");
    }
    
    SECTION("left constant location subtractions generates correctly") {
        std::string program = "let x = 4; print(x - 12)";
        code_generator_test_helper(program, "-8\n");
    }
    
    SECTION("right constant location subtractions generates correctly") {
        std::string program = "let x = 4; print(12 - x)";
        code_generator_test_helper(program, "8\n");
    }
    
    SECTION("all constant location subtractions generates correctly") {
        std::string program = "let x = 4; let y = 12; print(x - y)";
        code_generator_test_helper(program, "-8\n");
    }
    
    // Multiplication
    
    SECTION("constant value multiplications generates correctly") {
        std::string program = "print(4 * 12)";
        code_generator_test_helper(program, "48\n");
    }
    
    SECTION("left constant location multiplications generates correctly") {
        std::string program = "let x = 4; print(x * 12)";
        code_generator_test_helper(program, "48\n");
    }
    
    SECTION("right constant location multiplications generates correctly") {
        std::string program = "let x = 4; print(12 * x)";
        code_generator_test_helper(program, "48\n");
    }
    
    SECTION("all constant location multiplications generates correctly") {
        std::string program = "let x = 4; let y = 12; print(x * y)";
        code_generator_test_helper(program, "48\n");
    }
    
    // Operator Precedence
    
    SECTION("natural precedence generates correctly") {
        std::string program = "let x = 4 + 3 * 4 - 8; print(x)";
        code_generator_test_helper(program, "8\n");
        
        program = "let y = 4; let z = 3; let a = 8; let x = y + z * y - a; print(x)";
        code_generator_test_helper(program, "8\n");
    }
    
    SECTION("forced precedence generates correctly") {
        std::string program = "let x = (4 + 3) * 4 - 8; print(x)";
        code_generator_test_helper(program, "20\n");
        
        program = "let y = 4; let z = 3; let a = 8; let x = (y + z) * y - a; print(x)";
        code_generator_test_helper(program, "20\n");
        
        
        program = "let x = 4 + 3 * (4 - 8); print(x)";
        code_generator_test_helper(program, "-8\n");
        
        program = "let y = 4; let z = 3; let a = 8; let x = y + z * (y - a); print(x)";
        code_generator_test_helper(program, "-8\n");
        
        
        program = "let x = (4 + 3) * (4 - 8); print(x)";
        code_generator_test_helper(program, "-28\n");
        
        program = "let y = 4; let z = 3; let a = 8; let x = (y + z) * (y - a); print(x)";
        code_generator_test_helper(program, "-28\n");
    }
    
    // Chained Expressions
    
    SECTION("chained subtrees generate correctly") {
        std::string program =
            "print(4 + (3 + (4 + (3 + (4 + (3 + (4 + (3 + (4 + (3 + (4 + 3)))))))))))";
        code_generator_test_helper(program, "42\n");
        
        program =
            "let y = 3; let z = 4; print(z + (y + (z + (y + (z + (y + (z + (y + (z + (y + (z + y)))))))))))";
        code_generator_test_helper(program, "42\n");
    }
}

#pragma mark Assign Blocks

TEST_CASE("assign blocks generate correct code") {
    
    SECTION("integer assignment generates correctly") {
        std::string program = "var x = 4; print(x); x = 3; print(x)";
        code_generator_test_helper(program, "4\n3\n");
    }
    
    SECTION("constant-offset variable integer assignment generates correctly") {
        std::string program = "var x = 4; var y = 3; y = x; print(y)";
        code_generator_test_helper(program, "4\n");
    }

    SECTION("constant-offset constant integer assignment generates correctly") {
        std::string program = "let x = 4; var y = 3; y = x; print(y)";
        code_generator_test_helper(program, "4\n");
    }
}

#pragma mark Comparison Blocks

TEST_CASE("comparison blocks generate correct code") {
    
    // Both Constant
    
    SECTION("equals constant comparison generates correctly") {
        std::string program = "if 1 == 1 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 == 2 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("not equals constant comparison generates correctly") {
        std::string program = "if 1 != 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 != 1 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than constant comparison generates correctly") {
        std::string program = "if 1 < 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 2 < 1 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than equals constant comparison generates correctly") {
        std::string program = "if 2 <= 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 2 <= 1 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than constant comparison generates correctly") {
        std::string program = "if 3 > 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 2 > 3 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than equals constant comparison generates correctly") {
        std::string program = "if 2 >= 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 0 >= 1 { print(1) }";
        code_generator_test_helper(program, "");
    }

    // Left Non-Constant
    
    SECTION("equals right constant comparison generates correctly") {
        std::string program = "let x = 1; if x == 1 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if x == 2 { print(1) }";
        code_generator_test_helper(program, "");
    }

    SECTION("not equals right constant comparison generates correctly") {
        std::string program = "let x = 1; if x != 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if x != 1 { print(1) }";
        code_generator_test_helper(program, "");
    }

    SECTION("less than right constant comparison generates correctly") {
        std::string program = "let x = 1; if x < 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 2; if x < 1 { print(1) }";
        code_generator_test_helper(program, "");
    }

    SECTION("less than equals right constant comparison generates correctly") {
        std::string program = "let x = 2; if x <= 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 2; if x <= 1 { print(1) }";
        code_generator_test_helper(program, "");
    }

    SECTION("greater than right constant comparison generates correctly") {
        std::string program = "let x = 3; if x > 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 2; if x > 3 { print(1) }";
        code_generator_test_helper(program, "");
    }

    SECTION("greater than equals right constant comparison generates correctly") {
        std::string program = "let x = 2; if x >= 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 0; if x >= 1 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    // Right Non-Constant
    
    SECTION("equals left constant comparison generates correctly") {
        std::string program = "let x = 1; if 1 == x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 2; if 1 == x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("not equals left constant comparison generates correctly") {
        std::string program = "let x = 2; if 1 != x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 1 != x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than left constant comparison generates correctly") {
        std::string program = "let x = 2; if 1 < x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 2 < x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than equals left constant comparison generates correctly") {
        std::string program = "let x = 2; if 2 <= x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 2 <= x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than left constant comparison generates correctly") {
        std::string program = "let x = 2; if 3 > x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 3; if 2 > x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than equals left constant comparison generates correctly") {
        std::string program = "let x = 2; if 2 >= x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 0 >= x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    // Both Non-Constant
    
    SECTION("equals non-constant comparison generates correctly") {
        std::string program = "let x = 1; if 1 == x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 2; if 1 == x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("not equals non-constant comparison generates correctly") {
        std::string program = "let x = 2; if 1 != x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 1 != x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than non-constant comparison generates correctly") {
        std::string program = "let x = 2; if 1 < x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 2 < x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("less than equals non-constant comparison generates correctly") {
        std::string program = "let x = 2; if 2 <= x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 2 <= x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than non-constant comparison generates correctly") {
        std::string program = "let x = 2; if 3 > x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 3; if 2 > x { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("greater than equals non-constant comparison generates correctly") {
        std::string program = "let x = 2; if 2 >= x { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; if 0 >= x { print(1) }";
        code_generator_test_helper(program, "");
    }
}

#pragma mark If Blocks

TEST_CASE("if blocks generate correct code") {
    
    SECTION("constant conditions generate correct code") {
        std::string program = "if 1 < 2 { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 > 2 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("non-constant conditions generate correct code") {
        std::string program = "let x = 1; let y = 2; if x < y { print(1) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; let y = 2; if x > y { print(1) }";
        code_generator_test_helper(program, "");
    }
}

TEST_CASE("if else blocks generate correct code") {
    
    SECTION("constant conditions generate correct code") {
        std::string program = "if 1 < 2 { print(1) } else { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 > 2 { print(1) } else { print(0) }";
        code_generator_test_helper(program, "0\n");
    }
    
    SECTION("non-constant conditions generate correct code") {
        std::string program = "let x = 1; let y = 2; if x < y { print(1) } else { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; let y = 2; if x > y { print(1) } else { print(0) }";
        code_generator_test_helper(program, "0\n");
    }
}

TEST_CASE("if else if blocks generate correct code") {
    
    SECTION("constant conditions generate correct code") {
        std::string program = "if 1 < 2 { print(1) } else if 1 == 2 { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 > 2 { print(1) } else if 1 == 1 { print(2) }";
        code_generator_test_helper(program, "2\n");
        
        program = "if 1 > 2 { print(1) } else if 1 == 1 { print(2) } else if 1 < 2 { print(3) }";
        code_generator_test_helper(program, "2\n");
    }
    
    SECTION("non-constant conditions generate correct code") {
        std::string program = "let x = 1; let y = 2; if x < y { print(1) } else if x == y { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; let y = 2; if x > y { print(1) } else if x == x { print(2) }";
        code_generator_test_helper(program, "2\n");
        
        program =
            "let x = 1; let y = 2; if x > y { print(1) } else if x == x { print(2) } else if x < y { print(3) }";
        code_generator_test_helper(program, "2\n");
    }
}

TEST_CASE("if else if else blocks generate correct code") {
    
    SECTION("constant conditions generate correct code") {
        std::string program = "if 1 < 2 { print(1) } else { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "if 1 > 2 { print(1) } else { print(2) }";
        code_generator_test_helper(program, "2\n");
        
        program = "if 1 > 2 { print(1) } else if 1 == 2 { print(2) } else { print(3) }";
        code_generator_test_helper(program, "3\n");
    }
    
    SECTION("non-constant conditions generate correct code") {
        std::string program = "let x = 1; let y = 2; if x < y { print(1) } else { print(0) }";
        code_generator_test_helper(program, "1\n");
        
        program = "let x = 1; let y = 2; if x > y { print(1) } else { print(2) }";
        code_generator_test_helper(program, "2\n");
        
        program =
        "let x = 1; let y = 2; if x > y { print(1) } else if x == y { print(2) } else { print(3) }";
        code_generator_test_helper(program, "3\n");
    }
}

TEST_CASE("nested if statements generate correct code") {
    std::string program = "let x = 1; let y = 2; if x <= y { print(1); if x != y { print(2) } }";
    code_generator_test_helper(program, "1\n2\n");
}

#pragma mark While Blocks

// this assumes comparison works correctly
TEST_CASE("while blocks generate correct code") {
    
    SECTION("constant false conditions don't execute code") {
        std::string program = "while 1 > 2 { print(1) }";
        code_generator_test_helper(program, "");
    }
    
    SECTION("while blocks generate correct code code") {
        std::string program = "var x = 0; while x < 5 { print(x); x = x + 1 }";
        code_generator_test_helper(program, "0\n1\n2\n3\n4\n");
    }
}

#pragma mark Nested Control Statements

TEST_CASE("nested control statements generate correct code") {
    SECTION("while nested inside an if block generates correct code code") {
        std::string program = "if 1 < 2 { var x = 4; while x < 8 { print(x); x = x + 1 } }";
        code_generator_test_helper(program, "4\n5\n6\n7\n");
    }
}

TEST_CASE("declarations inside control statements generate correct code") {
    std::string program = "let x = 3; if 1 < 2 { let x = 4; print(x) }; print(x)";
    code_generator_test_helper(program, "4\n3\n");
}