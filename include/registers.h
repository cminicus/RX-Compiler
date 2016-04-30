//
//  registers.h
//  RX Compiler
//
//  Created by Clayton Minicus on 4/22/16.
//  Copyright © 2016 Clayton Minicus. All rights reserved.
//

#ifndef registers_h
#define registers_h

// 32 bit registers

#define eax "%eax"
#define ebx "%ebx"
#define ecx "%ecx"
#define edx "%edx"
#define esp "%esp"
#define ebp "%ebp"

#define edi "%edi"
#define esi "%esi"

#define _eax_ "(%eax)"
#define _ebx_ "(%ebx)"
#define _ecx_ "(%ecx)"
#define _edx_ "(%edx)"
#define _esp_ "(%esp)"
#define _ebp_ "(%ebp)"

#define _edi_ "(%edi)"
#define _esi_ "(%esi)"

#define _eax(x) "-" + std::to_string(x) + "(%eax)"
#define _ebx(x) "-" + std::to_string(x) + "(%ebx)"
#define _ecx(x) "-" + std::to_string(x) + "(%ecx)"
#define _edx(x) "-" + std::to_string(x) + "(%edx)"
#define _esp(x) "-" + std::to_string(x) + "(%esp)"
#define _ebp(x) "-" + std::to_string(x) + "(%ebp)"

// 64 bit registers

#define rax "%rax"
#define rbx "%rbx"
#define rcx "%rcx"
#define rdx "%rdx"
#define rsp "%rsp"
#define rbp "%rbp"

#define rdi "%rdi"
#define rsi "%rsi"

#define _rax_ "(%rax)"
#define _rbx_ "(%rbx)"
#define _rcx_ "(%rcx)"
#define _rdx_ "(%rdx)"
#define _rsp_ "(%rsp)"
#define _rbp_ "(%rbp)"

#define _rdi_ "(%rdi)"
#define _rsi_ "(%rsi)"

#define _rax(x) "-" + std::to_string(x) + "(%rax)"
#define _rbx(x) "-" + std::to_string(x) + "(%rbx)"
#define _rcx(x) "-" + std::to_string(x) + "(%rcx)"
#define _rdx(x) "-" + std::to_string(x) + "(%rdx)"
#define _rsp(x) "-" + std::to_string(x) + "(%rsp)"
#define _rbp(x) "-" + std::to_string(x) + "(%rbp)"

// Misc

#define _rip(x) x + "(%rip)"

#define _constant(x) "$" + std::to_string(x)

#endif /* registers_h */
