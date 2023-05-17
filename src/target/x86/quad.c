// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * quad.c -- x86 target code generation for quads
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/private/target.h>
#include <jkcc/target/x86/quad.h>
#include <jkcc/target/x86/util.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/ir.h>


int (*const target_x86_quad[IR_QUAD_TOTAL])(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args) = {
	[IR_QUAD_ALLOCA] = target_x86_quad_alloca,
	[IR_QUAD_ARG]    = NULL,
	[IR_QUAD_BINOP]  = target_x86_quad_binop,
	[IR_QUAD_BR]     = NULL,
	[IR_QUAD_CALL]   = NULL,
	[IR_QUAD_CMP]    = NULL,
	[IR_QUAD_LOAD]   = NULL,
	[IR_QUAD_MOV]    = NULL,
	[IR_QUAD_RET]    = NULL,
	[IR_QUAD_STORE]  = NULL,
};


int target_x86_quad_alloca(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;
	(void) args;

	// NOTE: we prealloc all our stack space

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	return 0;
}

int target_x86_quad_binop(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_binop_t *binop = OFFSETOF_IR_QUAD(quad, ir_quad_binop_t);

	SET_EAX(target_x86_util_ebp_offset(binop->lhs, args));
	SET_EDX(target_x86_util_ebp_offset(binop->rhs, args));

	switch (binop->op) {
		case IR_QUAD_BINOP_ADD:
			ADDL;
			break;

		case IR_QUAD_BINOP_SUB:
			SUBL;
			break;

		case IR_QUAD_BINOP_MUL:
			IMUL;
			break;

		case IR_QUAD_BINOP_DIV:
			IDIV;
			MOVL("%eax", "%edx");
			break;

		case IR_QUAD_BINOP_MOD:
			IDIV;
			break;

		case IR_QUAD_BINOP_AND:
			AND;
			break;

		case IR_QUAD_BINOP_OOR:
			OR;
			break;

		case IR_QUAD_BINOP_EOR:
			XOR;
			break;

		case IR_QUAD_BINOP_LSL:
			SAL;
			break;

		case IR_QUAD_BINOP_LSR:
			SAR;
			break;

		default:
			return -1;
	}
	STORE_RESULT(target_x86_util_ebp_offset(binop->dst, args));

	return 0;
}
