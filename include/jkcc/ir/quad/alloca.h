// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alloca.h -- stack allocation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_ALLOCA_H
#define JKCC_IR_QUAD_ALLOCA_H


#include <jkcc/ir/ir.h>

#include <stdint.h>
#include <stdio.h>


typedef struct ir_quad_alloc_s {
	uintptr_t     dst;
	ir_reg_type_t type;
	size_t        align;
	ir_quad_t     ir_quad;
} ir_quad_alloca_t;


void ir_quad_alloca_fprint(
	FILE          *stream,
	ir_quad_t     *ir_quad);
int ir_quad_alloca_gen(
	ir_quad_t    **ir_quad,
	uintptr_t      dst,
	ir_reg_type_t  type);


#endif  /* JKCC_IR_QUAD_ALLOCA_H */
