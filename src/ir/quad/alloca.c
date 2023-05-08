// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alloca.c -- stack allocation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/alloca.h>
#include <jkcc/ir/ir.h>

#include <jkcc/ir/quad.h>


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
