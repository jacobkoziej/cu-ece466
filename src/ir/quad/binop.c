// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * binop.c -- binary operation quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/binop.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stdint.h>
#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_binop_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_binop_t);

	ir_reg_fprint(stream, quad->dst);
	fprintf(stream, " = ");

	const char *op;
	switch (quad->op) {
		case IR_QUAD_BINOP_ADD:
			op = "add";
			break;

		case IR_QUAD_BINOP_SUB:
			op = "sub";
			break;

		case IR_QUAD_BINOP_MUL:
			op = "mul";
			break;

		case IR_QUAD_BINOP_DIV:
			op = "div";
			break;

		case IR_QUAD_BINOP_MOD:
			op = "mod";
			break;

		case IR_QUAD_BINOP_AND:
			op = "and";
			break;

		case IR_QUAD_BINOP_OOR:
			op = "oor";
			break;

		case IR_QUAD_BINOP_EOR:
			op = "eor";
			break;

		case IR_QUAD_BINOP_LSL:
			op = "lsl";
			break;

		case IR_QUAD_BINOP_LSR:
			op = "lsr";
			break;

		default:
			op = "(unknown)";
			break;
	}

	fprintf(stream, "%s ", op);
	ir_reg_type_fprint(stream, IR_REG_TYPE_I32);
	fprintf(stream, " ");
	ir_reg_fprint(stream, quad->lhs);
	fprintf(stream, ", ");
	ir_reg_fprint(stream, quad->rhs);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_binop_gen(
	ir_quad_t          **ir_quad,
	uintptr_t            dst,
	ir_quad_binop_op_t   op,
	uintptr_t            lhs,
	uintptr_t            rhs)
{
	IR_QUAD_INIT(ir_quad_binop_t);

	quad->dst = dst;
	quad->op  = op;
	quad->lhs = lhs;
	quad->rhs = rhs;

	IR_QUAD_RETURN(IR_QUAD_BINOP);
}
