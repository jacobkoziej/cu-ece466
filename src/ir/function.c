// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function.c -- function ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/function.h>
#include <jkcc/ir/ir.h>

#include <stdio.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>
#include <jkcc/string.h>


ir_function_t *ir_function_alloc(void)
{
	return calloc(1, sizeof(ir_function_t));
}

void ir_function_fprint(FILE *stream, ir_function_t *ir_function)
{
	fprintf(stream, "define ");
	{
		ast_t *return_type = ast_function_get_return_type(
			ir_function->declaration);

		const char* storage_type;

retry_return_type:
		switch (*return_type) {
			case AST_POINTER:
				return_type
					= ast_pointer_get_pointer(return_type);
				goto retry_return_type;

			case AST_ARRAY:
				return_type = ast_array_get_type(return_type);
				goto retry_return_type;

			case AST_TYPE:;
				ast_type_t *type = OFFSETOF_AST_NODE(
					return_type,
					ast_type_t);

				switch (type->storage_class_specifier) {
					case AST_STORAGE_CLASS_SPECIFIER_STATIC:
						storage_type = "internal";
						break;

					default:
						storage_type = "dso_local";
						break;
				}

				break;

			default:
				storage_type = "(unknown)";
				break;
		}

		fprintf(stream, "%s ", storage_type);
	}

	ir_reg_type_fprint(stream, ir_function->return_type);
	fprintf(stream, " @");
	{
		const string_t *identifier = ast_identifier_get_string(
			ast_function_get_identifier(ir_function->declaration));

		fprintf(stream, "%s", identifier->head);
	}

	fprintf(stream, "(");
	if (ir_function->argv) {
		ast_t **declaration = ir_function->argv->buf;
		for (size_t i = 0; i < ir_function->argv->use; i++) {
			uintptr_t key = (uintptr_t) ast_declaration_get_type(
				declaration[i]);

			void *val;

			ht_get(
				&ir_function->reg.type,
				&key,
				sizeof(key),
				&val);

			ir_reg_type_t type = (uintptr_t) val;
			ir_reg_type_fprint(stream, type);
			fprintf(stream, " ");

			ht_get(
				&ir_function->reg.lookup,
				&key,
				sizeof(key),
				&val);

			uintptr_t reg = (uintptr_t) val;
			ir_reg_fprint(stream, reg);

			if (i < ir_function->argv->use - 1)
				fprintf(stream, ", ");
		}
	}
	fprintf(stream, ") {\n");

	ir_bb_t **ir_bb = ir_function->bb.buf;
	for (size_t i = 0; i < ir_function->bb.use; i++)
		ir_bb_fprint(stream, ir_bb[i]);

	fprintf(stream, "}\n");
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
	int ir_error = IR_ERROR_NOMEM;

	ir_function->declaration = ast_function;

	vector_t *list;
	{
		ast_t *ast_list = ast_function_get_body(ast_function);

		if (!ast_list) return IR_ERROR_EMPTY_FUNCTION_BODY;

		list = ast_list_get_list(ast_list);
	}

	ir_function->return_type
		= ir_reg_type_gen(ast_function_get_return_type(ast_function));

	if (ht_init(&ir_function->reg.lookup, 0)) return IR_ERROR_NOMEM;
	if (ht_init(&ir_function->reg.type, 0)) goto error_ht_init_reg_type;

	// reset registers
	ir_context->current.dst = 0;

	{
		ast_t *ast_list;

		ast_list = ast_function_get_parameter_list(ast_function);
		if (ast_list) goto argv_done;

		ast_list = ast_function_get_declaration_list(ast_function);

argv_done:
		ir_function->argv = (ast_list)
			? ast_list_get_list(ast_list)
			: NULL;
	}

	if (ir_function->argv) {
		ast_t **declaration = ir_function->argv->buf;
		for (size_t i = 0; i < ir_function->argv->use; i++) {
			ast_t *ast_type
				= ast_declaration_get_type(declaration[i]);

			ir_reg_type_t type = ir_reg_type_gen(ast_type);

			uintptr_t key = (uintptr_t) ast_type;
			uintptr_t val = ir_context->current.dst++;

			if (ht_insert(
				&ir_function->reg.lookup,
				&key,
				sizeof(key),
				(void*) val)) goto error_ht_insert_reg_lookup;

			key = val;
			val = type;
			if (ht_insert(
				&ir_function->reg.type,
				&key,
				sizeof(key),
				(void*) val)) goto error_ht_insert_reg_type;
		}
	}

	if (vector_init(&ir_function->bb, sizeof(ir_bb_t*), 0))
		goto error_vector_init_bb;

	ir_context->ir_bb = NULL;

	ast_t **statement = list->buf;
	for (size_t i = 0; i < list->use; i++) {
		int ret = ir_bb_statement_gen(ir_context, statement[i]);
		if (ret) {
			ir_error = ret;
			goto error_ir_bb_statement_gen;
		}
	}

	return 0;

error_ir_bb_statement_gen:
	{
		ir_bb_t **ir_bb = ir_function->bb.buf;
		for (size_t i = 0; i < ir_function->bb.use; i++)
			ir_bb_free(ir_bb[i]);

		vector_free(&ir_function->bb);
	}

error_vector_init_bb:
error_ht_insert_reg_type:
error_ht_insert_reg_lookup:
	ht_free(&ir_function->reg.type, NULL);

error_ht_init_reg_type:
	ht_free(&ir_function->reg.lookup, NULL);

	return ir_error;
}
