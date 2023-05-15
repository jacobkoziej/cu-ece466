// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * declaration.c -- declaration basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/declaration.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>


int ir_bb_declaration_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	uint_fast8_t storage_class = ast_declaration_get_storage_class(ast);

	switch (storage_class) {
		case AST_DECLARATION_AUTO:
			break;

		case AST_DECLARATION_STATIC:;
			int ret = ir_declaration(ir_context, ast);
			if (ret) return ret;

			return 0;

		default:
			return IR_ERROR_UNIMPLEMENTED_STORAGE_CLASS;
	}

	IR_BB_INIT;

	ast_t         *ast_type = ast_declaration_get_type(ast);
	ir_reg_type_t  reg_type = ir_reg_type_gen(ast_type);

	ir_quad_t *quad;
	int ret = ir_quad_alloca_gen(&quad, ir_context->current.dst, reg_type);
	if (ret) return ret;

	uintptr_t key = (uintptr_t) ast_type;
	uintptr_t val = ir_context->current.dst;

	if (ht_insert(
		&ir_context->ir_function->reg.lookup,
		&key,
		sizeof(key),
		(void*) val)) goto error_ht_insert_reg_lookup;

	key = val;
	val = reg_type;
	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		(void*) val)) goto error_ht_insert_reg_type;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	ir_context->result = ir_context->current.dst++;

	return 0;

error_vector_append_ir_bb_quad:
error_ht_insert_reg_type:
	ht_rm(&ir_context->ir_function->reg.lookup, &key, sizeof(key), NULL);

error_ht_insert_reg_lookup:
	free(quad);

	return IR_ERROR_NOMEM;
}
