// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cmp.c -- comparison quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/cmp.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>


int ir_quad_cmp_gen(ir_quad_t **ir_quad, uintptr_t lhs, uintptr_t rhs)
{
	IR_QUAD_INIT(ir_quad_cmp_t);

	quad->lhs = lhs;
	quad->rhs = rhs;

	IR_QUAD_RETURN(IR_QUAD_CMP);
}
