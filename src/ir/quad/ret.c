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
