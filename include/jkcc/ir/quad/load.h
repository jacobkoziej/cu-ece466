// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * load.h -- load quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_LOAD_H
#define JKCC_IR_QUAD_LOAD_H


#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


typedef struct ir_quad_load_s {
	uintptr_t     dst;
	ir_reg_type_t type;
	ir_location_t src;
	size_t        align;
	ir_quad_t     ir_quad;
} ir_quad_load_t;


void ir_quad_load_fprint(
	FILE           *stream,
	ir_quad_t      *ir_quad);
int ir_quad_load_gen(
	ir_quad_t     **ir_quad,
	uintptr_t       dst,
	ir_reg_type_t   type,
	ir_location_t  *src);


#endif  /* JKCC_IR_QUAD_LOAD_H */
