// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cmp.c -- comparison quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/cmp.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>
#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_cmp_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_cmp_t);

	fprintf(stream, "cmp ");
	ir_reg_fprint(stream, quad->lhs);
	fprintf(stream, ", ");
	ir_reg_fprint(stream, quad->rhs);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_cmp_gen(ir_quad_t **ir_quad, uintptr_t lhs, uintptr_t rhs)
{
	IR_QUAD_INIT(ir_quad_cmp_t);

	quad->lhs = lhs;
	quad->rhs = rhs;

	IR_QUAD_RETURN(IR_QUAD_CMP);
}
