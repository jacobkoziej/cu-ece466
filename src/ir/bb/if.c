// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * if.c -- if basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/if.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>


int ir_bb_if_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		size_t   id;
		ir_bb_t *bb;
	}
	expression      = {0},
	true_statement  = {0},
	false_statement = {0},
	exit            = {0};

	ast_t *ast_expression      = ast_if_get_expression(ast);
	ast_t *ast_true_statement  = ast_if_get_true_statement(ast);
	ast_t *ast_false_statement = ast_if_get_false_statement(ast);

	int        ret;
	ir_quad_t *quad;

	expression.bb = ir_bb_alloc(ir_context->current.bb);
	if (!expression.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &expression.bb))
		return IR_ERROR_NOMEM;

	expression.id = ir_context->current.bb++;

	IR_BB_FINISH(ret, &quad, expression.id);
	if (ret) return ret;

	true_statement.bb = ir_bb_alloc(ir_context->current.bb);
	if (!true_statement.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &true_statement.bb))
		return IR_ERROR_NOMEM;

	true_statement.id = ir_context->current.bb++;

	if (ast_false_statement) {
		false_statement.bb = ir_bb_alloc(ir_context->current.bb);
		if (!false_statement.bb) return IR_ERROR_NOMEM;

		if (vector_append(
			&ir_context->ir_function->bb,
			&false_statement.bb))
			return IR_ERROR_NOMEM;

		false_statement.id = ir_context->current.bb++;
	}

	exit.bb = ir_bb_alloc(ir_context->current.bb);
	if (!exit.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &exit.bb))
		return IR_ERROR_NOMEM;

	exit.id = ir_context->current.bb++;

	ir_context->br_true = true_statement.id;
	ir_context->br_false
		= (ast_false_statement)
		? false_statement.id
		: exit.id;
	ir_context->br_exit = exit.id;

	ir_context->ir_bb = expression.bb;
	ret = IR_BB_GEN(ir_context, ast_expression);
	if (ret) return ret;

	ir_context->ir_bb = true_statement.bb;
	ret = IR_BB_GEN(ir_context, ast_true_statement);
	if (ret) return ret;

	IR_BB_FINISH(ret, &quad, exit.id);
	if (ret) return ret;

	if (ast_false_statement) {
		ir_context->ir_bb = false_statement.bb;
		ret = IR_BB_GEN(ir_context, ast_false_statement);
		if (ret) return ret;

		IR_BB_FINISH(ret, &quad, exit.id);
		if (ret) return ret;
	}

	ir_context->ir_bb    = exit.bb;

	return 0;
}
