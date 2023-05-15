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
		goto error_vector_append_ir_quad_cmp;

	ir_quad_br_condition_t condition;

	uint_fast32_t operator = ast_binary_operator_get_operator(ast);
	switch (operator) {
		case AST_BINARY_OPERATOR_LESS_THAN:
			condition = IR_QUAD_BR_LT;
			break;

		case AST_BINARY_OPERATOR_GREATER_THAN:
			condition = IR_QUAD_BR_GT;
			break;

		case AST_BINARY_OPERATOR_LESS_THAN_OR_EQUAL:
			condition = IR_QUAD_BR_LE;
			break;

		case AST_BINARY_OPERATOR_GREATER_THAN_OR_EQUAL:
			condition = IR_QUAD_BR_GE;
			break;

		case AST_BINARY_OPERATOR_EQUALITY:
			condition = IR_QUAD_BR_EQ;
			break;

		case AST_BINARY_OPERATOR_INEQUALITY:
			condition = IR_QUAD_BR_NE;
			break;

		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

	ret = ir_quad_br_gen(&quad, condition, ir_context->br_true);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_quad_br_true;

	ret = ir_quad_br_gen(&quad, IR_QUAD_BR_AL, ir_context->br_false);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_quad_br_false;

	return 0;

error_vector_append_ir_quad_br_false:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_quad_br_true:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_quad_cmp:
	return IR_ERROR_NOMEM;
}
