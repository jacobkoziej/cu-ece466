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

#include <jkcc/ht.h>
#include <jkcc/ir.h>


void ir_quad_alloca_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_alloca_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = alloca ");
	ir_reg_type_fprint(stream, quad->type);
	fprintf(stream, ", ");
	ir_align_fprint(stream, quad->align);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_alloca_gen(
	ir_quad_t    **ir_quad,
	uintptr_t      dst,
	ir_reg_type_t  type)
{
	IR_QUAD_INIT(ir_quad_alloca_t);

	quad->dst  = dst;
	quad->type = type;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_ALLOCA);
}
