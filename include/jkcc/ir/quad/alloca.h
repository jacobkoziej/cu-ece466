// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alloca.h -- stack allocation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_ALLOCA_H
#define JKCC_IR_QUAD_ALLOCA_H


#include <jkcc/ir/ir.h>

#include <stdint.h>


typedef enum ir_quad_alloca_type_e {
	IR_QUAD_ALLOCA_TYPE_I32,
	IR_QUAD_ALLOCA_TYPE_PTR,
} ir_quad_alloca_type_t;


typedef struct ir_quad_alloc_s {
	uintptr_t             dst;
	ir_quad_alloca_type_t type;
	size_t                align;
	ir_quad_t             ir_quad;
} ir_quad_alloca_t;


#endif  /* JKCC_IR_QUAD_ALLOCA_H */
