// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cmp.c -- cmp basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/cmp.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>


int ir_bb_cmp_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		uintptr_t lhs;
		uintptr_t rhs;
	} cmp;

	int        ret;
	ir_quad_t *quad;

	ret = IR_BB_GEN(ir_context, ast_binary_operator_get_lhs(ast));
	if (ret) return ret;
	cmp.lhs = ir_context->result;

	ret = IR_BB_GEN(ir_context, ast_binary_operator_get_rhs(ast));
	if (ret) return ret;
	cmp.rhs = ir_context->result;

	ret = ir_quad_cmp_gen(&quad, cmp.lhs, cmp.rhs);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	return 0;

error_vector_append_ir_bb_quad:
	return IR_ERROR_NOMEM;
}
