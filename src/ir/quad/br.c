// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * br.c -- branch quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/br.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stddef.h>


int ir_quad_br_gen(
	ir_quad_t              **ir_quad,
	ir_quad_br_condition_t   condition,
	size_t                   bb)
{
	IR_QUAD_INIT(ir_quad_br_t);

	quad->condition = condition;
	quad->bb        = bb;

	IR_QUAD_RETURN(IR_QUAD_BR);
}
