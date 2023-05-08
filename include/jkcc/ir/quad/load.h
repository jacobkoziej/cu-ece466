// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * load.h -- load quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_LOAD_H
#define JKCC_IR_QUAD_LOAD_H


#include <jkcc/ir/ir.h>


typedef enum ir_quad_load_type_e {
	IR_QUAD_LOAD_TYPE_I32,
	IR_QUAD_LOAD_TYPE_PTR,
} ir_quad_load_type_t;


#endif  /* JKCC_IR_QUAD_LOAD_H */
