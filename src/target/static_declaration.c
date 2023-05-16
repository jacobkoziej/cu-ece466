// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_declaration.c -- static declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/static_declaration.h>
#include <jkcc/target/target.h>

#include <stdio.h>
#include <stddef.h>

#include <jkcc/ir.h>


int (*const target_static_declaration[TARGET_TOTAL])(
	FILE                    *stream,
	ir_static_declaration_t *static_declaration) = {
	[TARGET_X86] = target_x86_static_declaration,
};
