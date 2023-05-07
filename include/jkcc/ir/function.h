// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.h -- function ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_FUNCTION_H
#define JKCC_IR_FUNCTION_H


#include <jkcc/ir/ir.h>


ir_function_t *ir_function_alloc(
	void);
void ir_function_free(
	ir_function_t *ir_function);
int ir_function_gen(
	ir_context_t  *ir_context,
	ir_function_t *ir_function,
	ast_t         *ast_function);


#endif  /* JKCC_IR_FUNCTION_H */
