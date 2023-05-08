// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * call.h -- call quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_CALL_H
#define JKCC_IR_QUAD_CALL_H


#include <jkcc/ir/ir.h>

#include <stdint.h>


typedef struct ir_quad_call_s {
	uintptr_t     dst;
	ir_reg_type_t type;
	ir_location_t src;
	ir_quad_t     ir_quad;
} ir_quad_call_t;


#endif  /* JKCC_IR_QUAD_CALL_H */
