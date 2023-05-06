// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.c -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir.h>
#include <jkcc/private/ir.h>

#include <stddef.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/vector.h>


ir_unit_t *ir_unit_alloc(void)
{
	ir_unit_t *ir_unit = malloc(sizeof(*ir_unit));
	if (!ir_unit) return NULL;

	if (ir_unit_init(ir_unit)) goto error_ir_unit_init;

	return ir_unit;

error_ir_unit_init:
	free(ir_unit);

	return NULL;
}

void ir_unit_free(ir_unit_t *ir_unit)
{
	if (!ir_unit) return;

	ir_unit_deinit(ir_unit);

	free(ir_unit);
}

int ir_unit_gen(ir_unit_t *ir_unit, ast_t *ast)
{
	int ir_error = IR_ERROR_NOMEM;

	vector_t *list;
	{
		ast_translation_unit_t *t_unit
			= OFFSETOF_AST_NODE(ast, ast_translation_unit_t);

		if (!t_unit->external_declaration)
			return IR_ERROR_EMPTY_TRANSLATION_UNIT;

		list = ast_list_get_list(t_unit->external_declaration);
	}

	ast_t **declaration_list = list->buf;
	for (size_t i = 0; i < list->use; i++)
		switch (*declaration_list[i]) {
			case AST_DECLARATION:
				break;

			case AST_FUNCTION:
				break;

			case AST_LIST: {
				vector_t *list = ast_list_get_list(
					declaration_list[i]);

				// TODO: add distinction
				// between static & extern
				ast_t **declaration = list->buf;
				for (size_t i = 0; i < list->use; i++)
					if (vector_append(
						&ir_unit->extern_declaration,
						&declaration[i])) goto error;

				break;
			}

			default:
				goto error_unknown_ast_node;
		}

	return 0;

error_unknown_ast_node:
	ir_error = IR_ERROR_EMPTY_TRANSLATION_UNIT;

	goto error;

error:
	{
		ir_function_t **ir_function = ir_unit->function.buf;
		for (size_t i = 0; i < ir_unit->function.use; i++)
			(void) ir_function;
	}

	return ir_error;
}

void ir_unit_deinit(ir_unit_t *ir_unit)
{
	if (!ir_unit) return;

	ir_function_t *ir_function = ir_unit->function.buf;
	for (size_t i = 0; i < ir_unit->function.use; i++)
		(void) ir_function;

	vector_free(&ir_unit->extern_declaration);
	vector_free(&ir_unit->static_declaration);
	vector_free(&ir_unit->function);
}

int ir_unit_init(ir_unit_t *ir_unit)
{
	if (vector_init(
		&ir_unit->extern_declaration,
		sizeof(ast_t*),
		0)) return -1;

	if (vector_init(
		&ir_unit->extern_declaration,
		sizeof(ir_static_declaration_t),
		0)) goto error_vector_init_static_declaration;

	if (vector_init(
		&ir_unit->function,
		sizeof(ir_function_t),
		0)) goto error_vector_init_function;

	return 0;

error_vector_init_function:
	vector_free(&ir_unit->static_declaration);

error_vector_init_static_declaration:
	vector_free(&ir_unit->extern_declaration);

	return -1;
}
