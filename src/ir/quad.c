// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * quad.c -- quad ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad.h>
#include <jkcc/ir/ir.h>

#include <stdio.h>


void (*const ir_quad_fprint[IR_QUAD_TOTAL])(
	FILE      *stream,
	ir_quad_t *ir_quad) = {
	[IR_QUAD_ALLOCA] = ir_quad_alloca_fprint,
	[IR_QUAD_BINOP]  = ir_quad_binop_fprint,
	[IR_QUAD_BR]     = ir_quad_br_fprint,
	[IR_QUAD_CALL]   = ir_quad_call_fprint,
	[IR_QUAD_CMP]    = ir_quad_cmp_fprint,
	[IR_QUAD_LOAD]   = ir_quad_load_fprint,
	[IR_QUAD_MOV]    = ir_quad_mov_fprint,
	[IR_QUAD_STORE]  = ir_quad_store_fprint,
};
