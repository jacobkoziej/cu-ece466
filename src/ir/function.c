// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/function.h>
#include <jkcc/ir/ir.h>

#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>


ir_function_t *ir_function_alloc(void)
{
	return calloc(1, sizeof(ir_function_t));
}

void ir_function_free(ir_function_t *ir_function)
{
	if (!ir_function) return;

	(void) ir_function->bb;

	free(ir_function);
}

int ir_function_gen(
	ir_context_t  *ir_context,
	ir_function_t *ir_function,
	ast_t         *ast_function)
{
	(void) ir_context;
	(void) ir_function;

	vector_t *list;
	{
		ast_t *ast_list = ast_function_get_body(ast_function);

		if (ast_list) return IR_ERROR_EMPTY_FUNCTION_BODY;

		list = ast_list_get_list(ast_list);
	}

	ast_t **statement = list->buf;
	for (size_t i = 0; i < list->use; i++)
		(void) statement;

	return 0;
}
