// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * return.c -- return basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/return.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>
#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_return_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	IR_BB_INIT;

	ir_reg_type_t type;
	uintptr_t     src;

	int        ret;
	ir_quad_t *quad;

	ast_t *ast_expression = ast_return_get_expression(ast);

	if (ast_expression) {
		ret = IR_BB_GEN(ir_context, ast_expression);
		if (ret) return ret;
		src = ir_context->result;

		uintptr_t  key = src;
		void      *val;

		ht_get(
			&ir_context->ir_function->reg.type,
			&key,
			sizeof(key),
			&val);

		type = (ir_reg_type_t) val;
	} else {
		type = IR_REG_TYPE_I32;
		src  = UINTPTR_MAX;
	}

	ret = ir_quad_ret_gen(&quad, type, src);
	if (ret) return ret;

	if (vector_append(
		&ir_context->ir_bb->quad,
		&quad))
		goto error_vector_append_ir_quad_ret;

	return 0;

error_vector_append_ir_quad_ret:
	free(quad);

	return ret;
}
