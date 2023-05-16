// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binop.c -- binop basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/binop.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_binop_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		ir_quad_binop_op_t op;
		uintptr_t          lhs;
		uintptr_t          rhs;
	} binop = {0};

	uint_fast32_t operator = ast_binary_operator_get_operator(ast);
	switch (operator) {
		case AST_BINARY_OPERATOR_MULTIPLICATION:
			binop.op = IR_QUAD_BINOP_MUL;
			break;

		case AST_BINARY_OPERATOR_DIVISION:
			binop.op = IR_QUAD_BINOP_DIV;
			break;

		case AST_BINARY_OPERATOR_MODULO:
			binop.op = IR_QUAD_BINOP_MOD;
			break;

		case AST_BINARY_OPERATOR_ADDITION:
			binop.op = IR_QUAD_BINOP_ADD;
			break;

		case AST_BINARY_OPERATOR_SUBTRACTION:
			binop.op = IR_QUAD_BINOP_SUB;
			break;

		case AST_BINARY_OPERATOR_LBITSHIFT:
			binop.op = IR_QUAD_BINOP_LSL;
			break;

		case AST_BINARY_OPERATOR_RBITSHIFT:
			binop.op = IR_QUAD_BINOP_LSR;
			break;

		case AST_BINARY_OPERATOR_AND:
			binop.op = IR_QUAD_BINOP_AND;
			break;

		case AST_BINARY_OPERATOR_EXCLUSIVE_OR:
			binop.op = IR_QUAD_BINOP_EOR;
			break;

		case AST_BINARY_OPERATOR_INCLUSIVE_OR:
			binop.op = IR_QUAD_BINOP_OOR;
			break;

		case AST_BINARY_OPERATOR_LESS_THAN:
		case AST_BINARY_OPERATOR_GREATER_THAN:
		case AST_BINARY_OPERATOR_LESS_THAN_OR_EQUAL:
		case AST_BINARY_OPERATOR_GREATER_THAN_OR_EQUAL:
		case AST_BINARY_OPERATOR_EQUALITY:
		case AST_BINARY_OPERATOR_INEQUALITY:
			return ir_bb_cmp_gen(ir_context, ast);

		case AST_BINARY_OPERATOR_LOGICAL_AND:
			// TODO: cmp
			return IR_ERROR_UNKNOWN_AST_NODE;

		case AST_BINARY_OPERATOR_LOGICAL_OR:
			return ir_bb_cmp_or_gen(ir_context, ast);

		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

	IR_BB_INIT;

	int ret;

	ret = IR_BB_GEN(ir_context, ast_binary_operator_get_lhs(ast));
	if (ret) return ret;
	binop.lhs = ir_context->result;

	ret = IR_BB_GEN(ir_context, ast_binary_operator_get_rhs(ast));
	if (ret) return ret;
	binop.rhs = ir_context->result;

	ir_quad_t *quad;
	ret = ir_quad_binop_gen(
		&quad,
		ir_context->current.dst,
		binop.op,
		binop.lhs,
		binop.rhs);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_bb_quad;

	uintptr_t key = ir_context->current.dst;
	// TODO: determine result types
	uintptr_t val = IR_REG_TYPE_I32;

	if (ht_insert(
		&ir_context->ir_function->reg.type,
		&key,
		sizeof(key),
		(void*) val)) goto error_ht_insert_reg_type;

	ir_context->result = ir_context->current.dst++;

	return 0;

error_ht_insert_reg_type:
	vector_pop(&ir_context->ir_bb->quad, NULL);

error_vector_append_ir_bb_quad:
	free(quad);

	return IR_ERROR_NOMEM;
}
