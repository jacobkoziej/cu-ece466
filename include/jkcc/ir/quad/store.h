// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * store.h -- store quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_STORE_H
#define JKCC_IR_QUAD_STORE_H


#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


typedef struct ir_quad_store_s {
	uintptr_t     src;
	ir_reg_type_t type;
	uintptr_t     dst;
	size_t        align;
	ir_quad_t     ir_quad;
} ir_quad_store_t;


void ir_quad_store_fprint(
	FILE           *stream,
	ir_quad_t      *ir_quad);
int ir_quad_store_gen(
	ir_quad_t     **ir_quad,
	uintptr_t       src,
	ir_reg_type_t   type,
	uintptr_t       dst);


#endif  /* JKCC_IR_QUAD_STORE_H */
