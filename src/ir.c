// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.c -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir.h>

#include <jkcc/ast.h>
#include <jkcc/vector.h>


void ir_unit_deinit(ir_unit_t *ir_unit)
{
	if (!ir_unit) return;

	ir_function_t *ir_function = ir_unit->function.buf;
	for (size_t i = 0; i < ir_unit->function.use; i++)
		(void) ir_function;

	vector_free(&ir_unit->declaration);
	vector_free(&ir_unit->function);
}

int ir_unit_init(ir_unit_t *ir_unit)
{
	if (vector_init(&ir_unit->declaration, sizeof(ast_t*), 0)) return -1;

	if (vector_init(&ir_unit->function, sizeof(ir_function_t), 0))
		goto error_vector_init_function;

	return 0;

error_vector_init_function:
	vector_free(&ir_unit->declaration);

	return -1;
}
