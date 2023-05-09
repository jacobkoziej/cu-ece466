// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * statement.c -- statement basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/statement.h>
#include <jkcc/ir/ir.h>

#include <jkcc/ast.h>


int ir_bb_statement_gen(ir_context_t *ir_context, ast_t *statement)
{
	(void) ir_context;

	switch (*statement) {
		default:
			return IR_ERROR_UNKNOWN_AST_NODE;
	}

	return 0;
}
