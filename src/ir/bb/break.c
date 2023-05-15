// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * break.c -- break basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/break.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdlib.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_break_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	(void) ast;

	ir_quad_t *quad;
	int ret = ir_quad_br_gen(
		&quad,
		IR_QUAD_BR_AL,
		ir_context->br_loop_exit);
	if (ret) return ret;

	if (vector_append(&ir_context->ir_bb->quad, &quad))
		goto error_vector_append_ir_quad_br_true;

	return 0;

error_vector_append_ir_quad_br_true:
	free(quad);

	return IR_ERROR_NOMEM;
}
