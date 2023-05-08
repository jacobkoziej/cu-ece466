// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * br.h -- branch quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_BR_H
#define JKCC_IR_QUAD_BR_H


#include <jkcc/ir/ir.h>

#include <stddef.h>
#include <stdio.h>


typedef enum ir_quad_br_condition_e {
	IR_QUAD_BR_EQ,
	IR_QUAD_BR_NE,
	IR_QUAD_BR_HS,
	IR_QUAD_BR_LO,
	IR_QUAD_BR_MI,
	IR_QUAD_BR_PL,
	IR_QUAD_BR_VS,
	IR_QUAD_BR_VC,
	IR_QUAD_BR_HI,
	IR_QUAD_BR_LS,
	IR_QUAD_BR_GE,
	IR_QUAD_BR_LT,
	IR_QUAD_BR_GT,
	IR_QUAD_BR_LE,
	IR_QUAD_BR_AL,
	IR_QUAD_BR_NV,
} ir_quad_br_condition_t;

typedef struct ir_quad_br_s {
	ir_quad_br_condition_t condition;
	size_t                 bb;
	ir_quad_t              ir_quad;
} ir_quad_br_t;


void ir_quad_br_fprint(
	FILE                    *stream,
	ir_quad_t               *ir_quad);
int ir_quad_br_gen(
	ir_quad_t              **ir_quad,
	ir_quad_br_condition_t   condition,
	size_t                   bb);


#endif  /* JKCC_IR_QUAD_BR_H */
