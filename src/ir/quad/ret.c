// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ret.c -- ret quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/ret.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ir.h>


void ir_quad_ret_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_ret_t);

	fprintf(stream, "ret ");
	if (quad->src == UINTPTR_MAX) {
		fprintf(stream, "void");
	} else {
		ir_reg_type_fprint(stream, quad->type);
		fprintf(stream, " ");
		ir_reg_fprint(stream, quad->src);
	}

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_ret_gen(
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       src)
{
	IR_QUAD_INIT(ir_quad_ret_t);

	quad->type = type;
	quad->src  = src;

	IR_QUAD_RETURN(IR_QUAD_RET);
}
