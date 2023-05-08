// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * call.c -- call quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/call.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>


int ir_quad_call_gen(
	ir_context_t   *ir_context,
	ir_quad_t     **ir_quad,
	ir_location_t  *src,
	ir_reg_type_t   type)
{
	IR_QUAD_INIT(ir_quad_call_t);

	quad->dst  = ++ir_context->current.dst;
	quad->type =   type;
	quad->src  =  *src;

	IR_QUAD_RETURN(IR_QUAD_CALL);
}
