// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * extern_declaration.h -- x86 extern declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_X86_EXTERN_DECLARATION_H
#define JKCC_TARGET_X86_EXTERN_DECLARATION_H


#include <stdio.h>

#include <jkcc/ast.h>


int target_x86_extern_declaration(FILE *stream, ast_t *ast);


#endif  /* JKCC_TARGET_X86_EXTERN_DECLARATION_H */
