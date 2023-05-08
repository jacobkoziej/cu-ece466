// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mov.c -- mov quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/mov.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>
#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_mov_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_mov_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = mov ");
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, " 0x%lx, ", quad->immediate);
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_mov_gen(
	ir_context_t   *ir_context,
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       immediate)
{
	IR_QUAD_INIT(ir_quad_mov_t);

	quad->dst       = ++ir_context->current.dst;
	quad->type      =   type;
	quad->immediate =   immediate;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_MOV);
}
