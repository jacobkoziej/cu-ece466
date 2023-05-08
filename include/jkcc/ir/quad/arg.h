// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * arg.h -- argument quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_ARG_H
#define JKCC_IR_QUAD_ARG_H


#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdint.h>


typedef struct ir_quad_arg_s {
	size_t        pos;
	uintptr_t     src;
	ir_reg_type_t type;
	ir_quad_t     ir_quad;
} ir_quad_arg_t;


#endif  /* JKCC_IR_QUAD_ARG_H */
