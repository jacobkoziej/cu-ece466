// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binop.c -- binary operation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/binop.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>


int ir_quad_binop_gen(
	ir_context_t        *ir_context,
	ir_quad_t          **ir_quad,
	ir_quad_binop_op_t   op,
	uintptr_t            lhs,
	uintptr_t            rhs)
{
	IR_QUAD_INIT(ir_quad_binop_t);

	quad->dst = ++ir_context->current.dst;
	quad->op  =   op;
	quad->lhs =   lhs;
	quad->rhs =   rhs;

	IR_QUAD_RETURN(IR_QUAD_BINOP);
}
