// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alloca.c -- stack allocation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/alloca.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdio.h>
#include <stdlib.h>

#include <jkcc/ir.h>


void ir_quad_alloca_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	ir_quad_alloca_t *quad = OFFSETOF_IR_QUAD(ir_quad, ir_quad_alloca_t);

	const char *type;
	switch (quad->type) {
		case IR_QUAD_ALLOCA_TYPE_I32:
			type = "i32";
			break;

		case IR_QUAD_ALLOCA_TYPE_PTR:
			type = "ptr";
			break;

		default:
			type = "(unknown)";
			break;
	}

	fprintf(
		stream,
		"\t%%%lu = alloca %s, align %lu\n",
		quad->dst,
		type,
		quad->align);
}

int ir_quad_alloca_gen(
	ir_context_t  *ir_context,
	ir_quad_t    **ir_quad,
	ast_t         *type)
{
	IR_QUAD_INIT(ir_quad_alloca_t);

	switch (*type) {
		case AST_POINTER:
			quad->type = IR_QUAD_ALLOCA_TYPE_PTR;
			break;

		default:
			// TODO: FIX THIS CRIME!
			// everything's an i32... sigh
			quad->type = IR_QUAD_ALLOCA_TYPE_I32;
			break;
	}

	quad->dst = ++ir_context->current.dst;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_ALLOCA);
}
