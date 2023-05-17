// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * target.h -- garbage target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_TARGET_H
#define JKCC_PRIVATE_TARGET_H

#define ADDL fprintf(stream, "\taddl\t %%eax, %%edx\n")

#define AND fprintf(stream, "\tand\t %%eax, %%edx\n")

#define CMPL fprintf(stream, "\tcmpl\t %%eax, %%edx\n")
#define CMPL fprintf(stream, "\tcmpl\t %%edx, %%eax\n")


#define DIRECTIVE(directive) fprintf(stream, "\t" directive "\n")

#define DIRECTIVE_ARG(directive, ...) fprintf(stream, "\t" directive "\n", __VA_ARGS__)

#define IDIV fprintf(stream, "\tidiv\t %%eax, %%edx\n")

#define IMUL fprintf(stream, "\timul\t %%eax, %%edx\n")

#define JE(location) fprintf(stream, "\tje\t.L%lu\n", location)

#define JG(location) fprintf(stream, "\tjg\t.L%lu\n", location)

#define JGE(location) fprintf(stream, "\tjge\t.L%lu\n", location)

#define JL(location) fprintf(stream, "\tjl\t.L%lu\n", location)

#define JLE(location) fprintf(stream, "\tjle\t.L%lu\n", location)

#define JNE(location) fprintf(stream, "\tjne\t.L%lu\n", location)

#define JMP(location) fprintf(stream, "\tjmp\t.L%lu\n", location)

#define LABEL(label) fprintf(stream, "%s:\n", label)

#define LEA_BB(bb) fprintf(stream, "\tlea\t.L%lu, %%edx\n", bb)

#define LEA_IDENTIFIER(identifier) fprintf(stream, "\tlea\t%s, %%edx\n", identifier)

#define LEAVE fprintf(stream, "\tleave\n")

#define LOCAL_LABEL(bb) fprintf(stream, ".L%lu:\n", bb)

#define MOVL(src, dst) fprintf(stream, "\tmovl\t%" src ", %" dst "\n")

#define MOVL_EDX(val) fprintf(stream, "\tmovl\t$%lu, %%edx\n", val)

#define SAL fprintf(stream, "\tsal\t %%eax, %%edx\n")

#define SAR fprintf(stream, "\tsar\t %%eax, %%edx\n")

#define SET_EAX(offset) fprintf(stream, "\tmovl\t-%d(%%ebp), %%eax\n", offset)

#define SET_EDX(offset) fprintf(stream, "\tmovl\t-%d(%%ebp), %%edx\n", offset)

#define STORE_RESULT(offset) fprintf(stream, "\tmovl\t%%edx, -%d(%%ebp)\n", offset)

#define STORE_EDX_DEREF_EAX fprintf(stream, "\tmovl\t%%edx, (%%eax)\n")

#define SUBL fprintf(stream, "\tsubl\t%%eax, %%edx\n")

#define OR fprintf(stream, "\tor\t %%eax, %%edx\n")

#define RET fprintf(stream, "\tret\n")

#define XOR fprintf(stream, "\txor\t %%eax, %%edx\n")


#endif  /* JKCC_PRIVATE_TARGET_H */
