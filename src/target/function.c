// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/function.h>
#include <jkcc/target/target.h>

#include <stdio.h>
#include <stddef.h>

#include <jkcc/ir.h>


int (*const target_function[TARGET_TOTAL])(
	FILE          *stream,
	ir_function_t *function) = {
	[TARGET_X86] = NULL,
};
