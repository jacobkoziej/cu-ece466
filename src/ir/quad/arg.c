// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * arg.c -- argument quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/arg.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_arg_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_arg_t);

	fprintf(stream, "arg %lu, ", quad->pos);
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, " ");
	ir_reg_fprint(stream, quad->src);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_arg_gen(
	ir_quad_t     **ir_quad,
	size_t          pos,
	uintptr_t       src,
	ir_reg_type_t   type)
{
	IR_QUAD_INIT(ir_quad_arg_t);

	quad->pos  = pos;
	quad->src  = src;
	quad->type = type;

	IR_QUAD_RETURN(IR_QUAD_ARG);
}
