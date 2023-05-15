// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.c -- symbol basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/symbol.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_symbol_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	uintptr_t  key = (uintptr_t) ast_identifier_get_type(ast);
	void      *val;

	if (ht_get(
		&ir_context->ir_function->reg.lookup,
		&key,
		sizeof(key),
		&val)
	) {
		// TODO: handle loading global variables
		return IR_ERROR_UNKNOWN_AST_NODE;
	}

	if (ir_context->lvalue) {
		ir_context->lvalue = false;
		ir_context->result = (uintptr_t) val;
		return 0;
	}

	ir_location_t src = {
		.type = IR_LOCATION_REG,
		.reg  = (uintptr_t) val,
	};

	key = (uintptr_t) val;
	ht_get(&ir_context->ir_function->reg.type, &key, sizeof(key), &val);

	ir_reg_type_t type = (uintptr_t) val;

	ir_quad_t *quad;
	int ret = ir_quad_load_gen(&quad, ir_context->current.dst, type, &src);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	key = ir_context->current.dst;
	// TODO: determine result types
	val = IR_REG_TYPE_I32;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		val)) goto error_ht_insert_reg_type;

	ir_context->result = ir_context->current.dst++;

	return 0;

error_ht_insert_reg_type:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_bb_quad:
	free(quad);

	return IR_ERROR_NOMEM;
}
