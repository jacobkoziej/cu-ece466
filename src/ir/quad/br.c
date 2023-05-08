// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * br.c -- branch quad
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/quad/br.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ir.h>


void ir_quad_br_fprint(FILE *stream, ir_quad_t *ir_quad)
{
	IR_QUAD_FPRINT_BEGIN(ir_quad_br_t);

	const char *condition;
	switch (quad->condition) {
		case IR_QUAD_BR_EQ:
			condition = "eq";
			break;

		case IR_QUAD_BR_NE:
			condition = "ne";
			break;

		case IR_QUAD_BR_HS:
			condition = "hs";
			break;

		case IR_QUAD_BR_LO:
			condition = "lo";
			break;

		case IR_QUAD_BR_MI:
			condition = "mi";
			break;

		case IR_QUAD_BR_PL:
			condition = "pl";
			break;

		case IR_QUAD_BR_VS:
			condition = "vs";
			break;

		case IR_QUAD_BR_VC:
			condition = "vc";
			break;

		case IR_QUAD_BR_HI:
			condition = "hi";
			break;

		case IR_QUAD_BR_LS:
			condition = "ls";
			break;

		case IR_QUAD_BR_GE:
			condition = "ge";
			break;

		case IR_QUAD_BR_LT:
			condition = "lt";
			break;

		case IR_QUAD_BR_GT:
			condition = "gt";
			break;

		case IR_QUAD_BR_LE:
			condition = "le";
			break;

		case IR_QUAD_BR_AL:
			condition = "al";
			break;

		case IR_QUAD_BR_NV:
			condition = "nv";
			break;

		default:
			condition = "(unknown)";
			break;
	}

	fprintf(stream, "br.%s .L%lu", condition, quad->bb);

	IR_QUAD_FPRINT_FINISH;
}

int ir_quad_br_gen(
	ir_quad_t              **ir_quad,
	ir_quad_br_condition_t   condition,
	size_t                   bb)
{
	IR_QUAD_INIT(ir_quad_br_t);

	quad->condition = condition;
	quad->bb        = bb;

	IR_QUAD_RETURN(IR_QUAD_BR);
}
