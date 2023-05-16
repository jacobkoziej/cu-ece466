// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cmp.c -- cmp basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/cmp.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_cmp_and_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		size_t   id;
		ir_bb_t *bb;
	} and;

	and.bb = ir_bb_alloc(ir_context->current.bb);
	if (!and.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &and.bb))
		return IR_ERROR_NOMEM;

	and.id = ir_context->current.bb++;

	ast_t *lhs = ast_binary_operator_get_lhs(ast);
	ast_t *rhs = ast_binary_operator_get_rhs(ast);

	// "push" onto the stack
	size_t br_true       = ir_context->br_true;
	bool   short_circuit = ir_context->short_circuit;

	ir_context->br_true       = and.id;
	ir_context->short_circuit = false;

	int ret;

	ret = ir_bb_cmp_gen(ir_context, lhs);
	if (ret) return ret;

	// "pop" from the stack
	ir_context->br_true       = br_true;
	ir_context->short_circuit = short_circuit;

	ir_context->ir_bb = and.bb;
	ret = ir_bb_cmp_gen(ir_context, rhs);
	if (ret) return ret;

	return 0;
}

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

		case AST_BINARY_OPERATOR_LOGICAL_AND:
			return ir_bb_cmp_and_gen(ir_context, ast);

		case AST_BINARY_OPERATOR_LOGICAL_OR:
			return ir_bb_cmp_or_gen(ir_context, ast);

		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

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

	ret = ir_quad_br_gen(&quad, condition, ir_context->br_true);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_quad_br_true;

	// remove unconditional branch for short-circuit operations
	if (!ir_context->short_circuit) {
		ret = ir_quad_br_gen(
			&quad,
			IR_QUAD_BR_AL,
			ir_context->br_false);
		if (ret) return ret;

		if (vector_append(&ir_context->ir_bb->quad, &quad))
			goto error_vector_append_ir_quad_br_false;
	}

	return 0;

error_vector_append_ir_quad_br_false:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_quad_br_true:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_quad_cmp:
	return IR_ERROR_NOMEM;
}

int ir_bb_cmp_or_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	int  ret;
	bool reset = false;

	ast_t *lhs = ast_binary_operator_get_lhs(ast);
	ast_t *rhs = ast_binary_operator_get_rhs(ast);

	if (!ir_context->short_circuit) {
		ir_context->short_circuit = true;
		reset                     = true;
	}

	ret = ir_bb_cmp_gen(ir_context, lhs);
	if (ret) return ret;

	if (reset) ir_context->short_circuit = false;

	ret = ir_bb_cmp_gen(ir_context, rhs);
	if (ret) return ret;

	return 0;
}
