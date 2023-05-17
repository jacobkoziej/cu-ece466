// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.c -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir.h>
#include <jkcc/private/ir.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


void ir_align_fprint(FILE *stream, size_t align)
{
	fprintf(stream, "align %lu", align);
}

int ir_declaration(ir_context_t *ir_context, ast_t *declaration)
{
	ir_static_declaration_t *ir_static_declaration;
	ast_t                   *type;

	if (*declaration == AST_STRING_LITERAL) goto string_literal;

	switch (ast_declaration_get_storage_class(declaration)) {
		case AST_DECLARATION_IMPLICIT_EXTERN:
			if (vector_append(
				&ir_context->ir_unit->extern_declaration,
				&declaration)) return IR_ERROR_NOMEM;

			type = ast_declaration_get_type(declaration);
			if (ht_insert(
				&ir_context->extern_declaration,
				&type,
				sizeof(ast_type_t*),
				(void*) declaration))
				goto error_ht_insert_ir_extern_declaration;

			break;

string_literal:
		case AST_DECLARATION_STATIC:
			ir_static_declaration = ir_static_declaration_alloc(
				ir_context,
				declaration);
			if (!ir_static_declaration) return IR_ERROR_NOMEM;

			if (vector_append(
				&ir_context->ir_unit->static_declaration,
				&ir_static_declaration))
				goto error_vector_append_ir_static_declaration;

			// small hack to get strings literals working
			type = (*declaration != AST_STRING_LITERAL)
				? ast_declaration_get_type(declaration)
				: declaration;
			if (ht_insert(
				&ir_context->static_declaration,
				&type,
				sizeof(type),
				(void*) ir_static_declaration))
				goto error_ht_insert_ir_static_declaration;

			break;

		default:
			return IR_ERROR_UNIMPLEMENTED_STORAGE_CLASS;
	}

	return 0;

error_ht_insert_ir_extern_declaration:
	vector_pop(&ir_context->ir_unit->extern_declaration, NULL);

	return IR_ERROR_NOMEM;

error_ht_insert_ir_static_declaration:
	vector_pop(&ir_context->ir_unit->static_declaration, NULL);

error_vector_append_ir_static_declaration:
	--ir_context->current.bb;
	free(ir_static_declaration);

	return IR_ERROR_NOMEM;
}

void ir_extern_declaration_symbol_fprint(FILE *stream, ast_t *declaration)
{
	const string_t *identifier = ast_identifier_get_string(
		ast_declaration_get_identifier(declaration));

	fprintf(stream, "@%s", identifier->head);
}

ir_static_declaration_t *ir_static_declaration_alloc(
	ir_context_t *ir_context,
	ast_t        *declaration)
{
	ir_static_declaration_t *ir_static_declaration
		= malloc(sizeof(*ir_static_declaration));

	if (!ir_static_declaration) return NULL;

	ir_static_declaration->bb          = ir_context->current.bb++;
	ir_static_declaration->declaration = declaration;

	return ir_static_declaration;
}

void ir_reg_fprint(FILE *stream, uintptr_t reg)
{
	fprintf(stream, "%%%lu", reg);
}

void ir_reg_type_fprint(FILE *stream, ir_reg_type_t type)
{
	const char *type_str;
	switch (type) {
		case IR_REG_TYPE_I32:
			type_str = "i32";
			break;

		case IR_REG_TYPE_PTR:
			type_str = "ptr";
			break;

		case IR_REG_TYPE_LEA:
			type_str = "lea";
			break;
	}

	fprintf(stream, "%s", type_str);
}

ir_reg_type_t ir_reg_type_gen(ast_t *type)
{
	ir_reg_type_t reg_type;

	switch (*type) {
		case AST_ARRAY:
		case AST_POINTER:
			reg_type = IR_REG_TYPE_PTR;
			break;

		default:
			// TODO: FIX THIS CRIME!
			// everything's an i32... sigh
			reg_type = IR_REG_TYPE_I32;
			break;
	}

	return reg_type;
}

void ir_static_declaration_symbol_fprint(
	FILE                    *stream,
	ir_static_declaration_t *declaration)
{
	fprintf(stream, "@.L%lu", declaration->bb);
}

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

void ir_unit_fprint(FILE *stream, ir_unit_t *ir_unit)
{
	ir_function_t **ir_function = ir_unit->function.buf;
	for (size_t i = 0; i < ir_unit->function.use; i++) {
		ir_function_fprint(stream, ir_function[i]);

		if (i < ir_unit->function.use - 1)
			fprintf(stream, "\n");
	}
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

	ir_context_t ir_context = {
		.ir_unit = ir_unit,
	};

	if (ht_init(&ir_context.extern_declaration, 0)) goto error;
	if (ht_init(&ir_context.static_declaration, 0)) goto error;

	ast_t **declaration_list = list->buf;
	for (size_t i = 0; i < list->use; i++)
		switch (*declaration_list[i]) {
			case AST_DECLARATION: {
				int ret = ir_declaration(
					&ir_context,
					declaration_list[i]);
				if (ret) {
					ir_error = ret;
					goto error;
				}

				break;
			}

			case AST_FUNCTION: {
				ir_function_t *ir_function
					= ir_function_alloc();
				if (!ir_function) goto error;

				if (vector_append(
					&ir_unit->function,
					&ir_function)
				) {
					ir_function_free(ir_function);
					goto error;
				}

				ir_context.ir_function = ir_function;

				int ret = ir_function_gen(
					&ir_context,
					ir_function,
					declaration_list[i]);
				if (ret) {
					ir_error = ret;
					goto error;
				}

				break;
			}

			case AST_LIST: {
				vector_t *list = ast_list_get_list(
					declaration_list[i]);

				ast_t **declaration = list->buf;
				for (size_t i = 0; i < list->use; i++) {
					int ret = ir_declaration(
						&ir_context,
						declaration[i]);
					if (ret) {
						ir_error = ret;
						goto error;
					}
				}

				break;
			}

			default:
				ir_error = IR_ERROR_UNKNOWN_AST_NODE;
				goto error;
		}

	return 0;

error:
	ht_free(&ir_context.static_declaration, free);

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

	ir_function_t **ir_function = ir_unit->function.buf;
	for (size_t i = 0; i < ir_unit->function.use; i++)
		ir_function_free(ir_function[i]);

	ir_static_declaration_t **ir_static_declaration
		= ir_unit->static_declaration.buf;
	for (size_t i = 0; i < ir_unit->static_declaration.use; i++)
		free(ir_static_declaration[i]);

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
		&ir_unit->static_declaration,
		sizeof(ir_static_declaration_t*),
		0)) goto error_vector_init_static_declaration;

	if (vector_init(
		&ir_unit->function,
		sizeof(ir_function_t*),
		0)) goto error_vector_init_function;

	return 0;

error_vector_init_function:
	vector_free(&ir_unit->static_declaration);

error_vector_init_static_declaration:
	vector_free(&ir_unit->extern_declaration);

	return -1;
}
