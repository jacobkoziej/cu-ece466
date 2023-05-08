// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * arg.c -- argument quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/arg.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>


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
