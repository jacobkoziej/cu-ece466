// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cmp.h -- comparison quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_CMP_H
#define JKCC_IR_QUAD_CMP_H


#include <jkcc/ir/ir.h>

#include <stdint.h>


typedef struct ir_quad_cmp_s {
	uintptr_t lhs;
	uintptr_t rhs;
	ir_quad_t ir_quad;
} ir_quad_cmp_t;


int ir_quad_cmp_gen(
	ir_quad_t **ir_quad,
	uintptr_t   lhs,
	uintptr_t   rhs);


#endif  /* JKCC_IR_QUAD_CMP_H */
