// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * store.c -- store quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/store.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>

#include <jkcc/ir.h>


int ir_quad_store_gen(
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       src,
	ir_location_t  *dst)
{
	IR_QUAD_INIT(ir_quad_store_t);

	quad->dst  = *dst;
	quad->type =  type;
	quad->src  =  src;

	// TODO: determine alignment
	quad->align = 0;

	IR_QUAD_RETURN(IR_QUAD_STORE);
}
