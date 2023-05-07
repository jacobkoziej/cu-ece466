// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/function.h>
#include <jkcc/ir/ir.h>

#include <stdlib.h>


ir_function_t *ir_function_alloc(void)
{
	return calloc(1, sizeof(ir_function_t));
}
