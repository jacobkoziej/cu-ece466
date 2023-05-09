// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mov.c -- mov basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/mov.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <jkcc/ast.h>
#include <jkcc/constant.h>
#include <jkcc/ir.h>


int ir_bb_mov_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	ir_reg_type_t reg_type;

	switch (*ast) {
		case AST_INTEGER_CONSTANT:
			reg_type = IR_REG_TYPE_I32;
			break;

		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

	IR_BB_INIT;

	const integer_constant_t *integer_constant
		= ast_integer_constant_get_integer_constant(ast);

	if (integer_constant->type != INT) return IR_ERROR_UNKNOWN_AST_NODE;

	ir_quad_t *quad;
	int ret = ir_quad_mov_gen(
		&quad,
		ir_context->current.dst,
		reg_type,
		integer_constant->INT);
	if (ret) return ret;

	++ir_context->current.dst;

	if (vector_append(
		&ir_context->ir_bb->quad,
		&quad)) goto error_vector_append_ir_bb_quad;

	return 0;

error_vector_append_ir_bb_quad:
	--ir_context->current.dst;

	free(quad);

	return IR_ERROR_NOMEM;
}
