// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * dereference.c -- dereference basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/dereference.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_dereference_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		ir_location_t src;
		ir_reg_type_t type;
	} load = {0};

	load.src.type = IR_LOCATION_REG;

	uintptr_t  key;
	void      *val;
	ir_quad_t *quad;
	int        ret;

	ast_t *operand = ast_dereference_get_operand(ast);

	if (*operand == AST_DEREFERENCE) {
		ir_bb_dereference_gen(ir_context, operand);

		load.src.reg = ir_context->result;
		load.type    = IR_REG_TYPE_PTR;

		goto load_value;
	}

	IR_BB_INIT;

	// if we're dereferencing, we're
	// going to generate a valid lvalue
	ir_context->lvalue = false;

	ret = IR_BB_GEN(ir_context, operand);
	if (ret) return ret;
	load.src.reg = ir_context->result;
	load.type    = IR_REG_TYPE_PTR;

load_value:
	ret = ir_quad_load_gen(
		&quad,
		ir_context->current.dst,
		load.type,
		&load.src);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	key = ir_context->current.dst;
	val = (void*) load.type;

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
