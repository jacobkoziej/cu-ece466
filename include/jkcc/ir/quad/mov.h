// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mov.h -- mov quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_MOV_H
#define JKCC_IR_QUAD_MOV_H


#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


typedef struct ir_quad_mov_s {
	uintptr_t     dst;
	ir_reg_type_t type;
	uintptr_t     immediate;
	size_t        align;
	ir_quad_t     ir_quad;
} ir_quad_mov_t;


void ir_quad_mov_fprint(
	FILE           *stream,
	ir_quad_t      *ir_quad);
int ir_quad_mov_gen(
	ir_context_t   *ir_context,
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       immediate);


#endif  /* JKCC_IR_QUAD_MOV_H */
