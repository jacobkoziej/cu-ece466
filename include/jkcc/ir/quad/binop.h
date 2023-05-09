// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binop.h -- binary operation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_QUAD_BINOP_H
#define JKCC_IR_QUAD_BINOP_H


#include <jkcc/ir/ir.h>

#include <stdint.h>
#include <stdio.h>


typedef enum ir_quad_binop_op_e {
	IR_QUAD_BINOP_ADD,
	IR_QUAD_BINOP_SUB,
	IR_QUAD_BINOP_MUL,
	IR_QUAD_BINOP_DIV,
	IR_QUAD_BINOP_MOD,
	IR_QUAD_BINOP_AND,
	IR_QUAD_BINOP_OOR,
	IR_QUAD_BINOP_EOR,
	IR_QUAD_BINOP_LSL,
	IR_QUAD_BINOP_LSR,
} ir_quad_binop_op_t;


typedef struct ir_quad_binop_s {
	uintptr_t          dst;
	ir_quad_binop_op_t op;
	uintptr_t          lhs;
	uintptr_t          rhs;
	ir_quad_t          ir_quad;
} ir_quad_binop_t;


void ir_quad_binop_fprint(
	FILE                *stream,
	ir_quad_t           *ir_quad);
int ir_quad_binop_gen(
	ir_quad_t          **ir_quad,
	uintptr_t            dst,
	ir_quad_binop_op_t   op,
	uintptr_t            lhs,
	uintptr_t            rhs);


#endif  /* JKCC_IR_QUAD_BINOP_H */
