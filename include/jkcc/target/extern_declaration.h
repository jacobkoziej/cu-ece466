// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * extern_declaration.h -- extern declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_EXTERN_DECLARATION_H
#define JKCC_TARGET_EXTERN_DECLARATION_H


#include <jkcc/target/target.h>
#include <jkcc/target/x86/extern_declaration.h>

#include <stdio.h>

#include <jkcc/ast.h>


extern int (*const target_extern_declaration[TARGET_TOTAL])(
	FILE  *stream,
	ast_t *ast);


#endif  /* JKCC_TARGET_EXTERN_DECLARATION_H */
