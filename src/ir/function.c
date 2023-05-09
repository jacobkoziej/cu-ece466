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
	ir_function->declaration = ast_function;

	vector_t *list;
	{
		ast_t *ast_list = ast_function_get_body(ast_function);

		if (!ast_list) return IR_ERROR_EMPTY_FUNCTION_BODY;

		list = ast_list_get_list(ast_list);
	}

	if (ht_init(&ir_context->reg.lookup, 0)) return IR_ERROR_NOMEM;
	if (ht_init(&ir_context->reg.type, 0)) goto error_ht_init_reg_type;

	// reset registers
	ir_context->current.dst = 0;

	vector_t *argv;
	{
		ast_t *ast_list;

		ast_list = ast_function_get_parameter_list(ast_function);
		if (ast_list) goto argv_done;

		ast_list = ast_function_get_declaration_list(ast_function);

argv_done:
		argv = (ast_list) ? ast_list_get_list(ast_list) : NULL;
	}

	if (argv) {
		ast_t **declaration = argv->buf;
		for (size_t i = 0; i < argv->use; i++) {
			ast_t *ast_type
				= ast_declaration_get_type(declaration[i]);

			ir_reg_type_t type;
			switch (*ast_type) {
				case AST_ARRAY:
				case AST_POINTER:
					type = IR_REG_TYPE_PTR;
					break;

				default:
					// TODO: FIX THIS CRIME!
					// everything's an i32... sigh
					type = IR_REG_TYPE_I32;
					break;
			}

			uintptr_t key = (uintptr_t) ast_type;
			uintptr_t val = ir_context->current.dst++;

			if (ht_insert(
				&ir_context->reg.lookup,
				&key,
				sizeof(key),
				(void*) val)) goto error_ht_insert_reg_lookup;

			val = type;
			if (ht_insert(
				&ir_context->reg.type,
				&key,
				sizeof(key),
				(void*) val)) goto error_ht_insert_reg_type;
		}
	}

	ast_t **statement = list->buf;
	for (size_t i = 0; i < list->use; i++)
		(void) statement;

	return 0;

error_ht_insert_reg_type:
error_ht_insert_reg_lookup:
	ht_free(&ir_context->reg.type, NULL);

error_ht_init_reg_type:
	ht_free(&ir_context->reg.lookup, NULL);

	return IR_ERROR_NOMEM;
}
