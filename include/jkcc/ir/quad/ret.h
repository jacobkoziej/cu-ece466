// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ret.h -- ret quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_RET_H
#define JKCC_IR_QUAD_RET_H


#include <jkcc/ir/ir.h>

#include <stdint.h>


typedef struct ir_quad_ret_s {
	ir_reg_type_t type;
	uintptr_t     src;
	ir_quad_t     ir_quad;
} ir_quad_ret_t;


int ir_quad_ret_gen(
	ir_quad_t     **ir_quad,
	ir_reg_type_t   type,
	uintptr_t       src);


#endif  /* JKCC_IR_QUAD_RET_H */
