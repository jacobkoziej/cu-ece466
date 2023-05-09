// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * symbol.c -- symbol basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/symbol.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/ht.h>
#include <jkcc/ir.h>


int ir_bb_symbol_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	uintptr_t  key = (uintptr_t) ast_identifier_get_type(ast);
	void      *val;

	if (ht_get(
		&ir_context->ir_function->reg.lookup,
		&key,
		sizeof(key),
		&val)
	) {
		// TODO: handle loading global variables
		return IR_ERROR_UNKNOWN_AST_NODE;
	}

	ir_context->result = (uintptr_t) val;

	return 0;
}
