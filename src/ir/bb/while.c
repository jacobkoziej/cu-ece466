// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * while.c -- while basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/while.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>


int ir_bb_while_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	struct {
		size_t   id;
		ir_bb_t *bb;
	} expression = {0}, statement = {0}, exit = {0};

	ast_t *ast_expression = ast_while_get_expression(ast);
	ast_t *ast_statement  = ast_while_get_statement(ast);

	int        ret;
	ir_quad_t *quad;

	IR_BB_INIT;

	expression.bb = ir_bb_alloc(ir_context->current.bb);
	if (!expression.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &expression.bb))
		return IR_ERROR_NOMEM;

	expression.id = ir_context->current.bb++;

	IR_BB_FINISH(ret, &quad, expression.id);
	if (ret) return ret;

	if (*ast_statement != AST_EMPTY) {
		statement.bb = ir_bb_alloc(ir_context->current.bb);
		if (!statement.bb) return IR_ERROR_NOMEM;

		if (vector_append(
			&ir_context->ir_function->bb,
			&statement.bb))
			return IR_ERROR_NOMEM;

		statement.id = ir_context->current.bb++;
	}

	exit.bb = ir_bb_alloc(ir_context->current.bb);
	if (!exit.bb) return IR_ERROR_NOMEM;

	if (vector_append(&ir_context->ir_function->bb, &exit.bb))
		return IR_ERROR_NOMEM;

	exit.id = ir_context->current.bb++;

	// since we maintain the context between calls
	// to IR_BR_GEN(), we need to "push" our stack
	size_t br_loop_expression = ir_context->br_loop_expression;
	size_t br_loop_exit       = ir_context->br_loop_exit;
	size_t br_true            = ir_context->br_true;
	size_t br_false           = ir_context->br_false;
	size_t br_exit            = ir_context->br_exit;

	ir_context->br_loop_expression = expression.id;
	ir_context->br_loop_exit       = exit.id;
	ir_context->br_true
		= (ast_statement)
		? statement.id
		: expression.id;
	ir_context->br_false = exit.id;
	ir_context->br_exit  = exit.id;

	ir_context->ir_bb = expression.bb;
	ret = IR_BB_GEN(ir_context, ast_expression);
	if (ret) return ret;

	if (*ast_statement != AST_EMPTY) {
		ir_context->ir_bb = statement.bb;
		ret = IR_BB_GEN(ir_context, ast_statement);
		if (ret) return ret;

		IR_BB_FINISH(ret, &quad, expression.id);
		if (ret) return ret;
	}

	// "popping" our stack
	ir_context->br_loop_expression = br_loop_expression;
	ir_context->br_loop_exit       = br_loop_exit;
	ir_context->br_true            = br_true;
	ir_context->br_false           = br_false;
	ir_context->br_exit            = br_exit;
	ir_context->ir_bb              = exit.bb;

	return 0;
}
