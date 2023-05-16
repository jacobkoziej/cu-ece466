// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * extern_declaration.c -- extern declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/extern_declaration.h>
#include <jkcc/target/target.h>

#include <stdio.h>
#include <stddef.h>

#include <jkcc/ast.h>


int (*const target_extern_declaration[TARGET_TOTAL])(
	FILE  *stream,
	ast_t *ast) = {
	[TARGET_X86] = target_x86_extern_declaration,
};
