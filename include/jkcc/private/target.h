// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * target.h -- garbage target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_TARGET_H
#define JKCC_PRIVATE_TARGET_H

#define ADDL fprintf(stream, "\taddl\t %%eax, %%edx\n")

#define AND fprintf(stream, "\tand\t %%eax, %%edx\n")

#define DIRECTIVE(directive) fprintf(stream, "\t" directive "\n")

#define DIRECTIVE_ARG(directive, ...) fprintf(stream, "\t" directive "\n", __VA_ARGS__)

#define IDIV fprintf(stream, "\tidiv\t %%eax, %%edx\n")

#define IMUL fprintf(stream, "\timul\t %%eax, %%edx\n")

#define LABEL(label) fprintf(stream, "%s:\n", label)

#define LOCAL_LABEL(bb) fprintf(stream, ".L%lu:\n", bb)

#define MOVL(src, dst) fprintf(stream, "\tmovl\t%" src ", %" dst "\n")

#define SAL fprintf(stream, "\tsal\t %%eax, %%edx\n")

#define SAR fprintf(stream, "\tsar\t %%eax, %%edx\n")

#define SET_EAX(offset) fprintf(stream, "\tmovl\t-%d(%%ebp), %%eax\n", offset)

#define SET_EDX(offset) fprintf(stream, "\tmovl\t-%d(%%ebp), %%edx\n", offset)

#define STORE_RESULT(offset) fprintf(stream, "\tmovl\t%%edx, -%d(%%ebp)\n", offset)

#define SUBL fprintf(stream, "\tsubl\t %%eax, %%edx\n")

#define OR fprintf(stream, "\tor\t %%eax, %%edx\n")

#define RET fprintf(stream, "\tret\n")

#define XOR fprintf(stream, "\txor\t %%eax, %%edx\n")


#endif  /* JKCC_PRIVATE_TARGET_H */
