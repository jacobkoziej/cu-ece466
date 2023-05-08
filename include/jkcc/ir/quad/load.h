// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * load.h -- load quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_LOAD_H
#define JKCC_IR_QUAD_LOAD_H


#include <jkcc/ir/ir.h>

#include <stdint.h>


typedef struct ir_quad_load_s {
	uintptr_t     dst;
	ir_reg_type_t type;
	ir_location_t src;
	size_t        align;
	ir_quad_t     ir_quad;
} ir_quad_load_t;


#endif  /* JKCC_IR_QUAD_LOAD_H */
