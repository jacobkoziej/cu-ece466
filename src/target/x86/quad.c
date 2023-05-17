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
	[IR_QUAD_BR]     = target_x86_quad_br,
	[IR_QUAD_CALL]   = NULL,
	[IR_QUAD_CMP]    = target_x86_quad_cmp,
	[IR_QUAD_LOAD]   = NULL,
	[IR_QUAD_MOV]    = target_x86_quad_mov,
	[IR_QUAD_RET]    = target_x86_quad_ret,
	[IR_QUAD_STORE]  = target_x86_quad_store,
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

int target_x86_quad_br(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;
	(void) args;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_br_t *br = OFFSETOF_IR_QUAD(quad, ir_quad_br_t);

	switch (br->condition) {
		case IR_QUAD_BR_EQ:
			JE(br->bb);
			break;

		case IR_QUAD_BR_NE:
			JNE(br->bb);
			break;

		case IR_QUAD_BR_GE:
			JGE(br->bb);
			break;

		case IR_QUAD_BR_LT:
			JL(br->bb);
			break;

		case IR_QUAD_BR_GT:
			JG(br->bb);
			break;

		case IR_QUAD_BR_LE:
			JLE(br->bb);
			break;

		case IR_QUAD_BR_AL:
			JMP(br->bb);
			break;

		default:
			return -1;
	}

	return 0;
}

int target_x86_quad_cmp(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_cmp_t *cmp = OFFSETOF_IR_QUAD(quad, ir_quad_cmp_t);

	SET_EAX(target_x86_util_ebp_offset(cmp->lhs, args));
	SET_EDX(target_x86_util_ebp_offset(cmp->rhs, args));
	CMPL;

	return 0;
}

int target_x86_quad_mov(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_mov_t *mov = OFFSETOF_IR_QUAD(quad, ir_quad_mov_t);

	MOVL_EDX(mov->immediate);
	STORE_RESULT(target_x86_util_ebp_offset(mov->dst, args));

	return 0;
}

int target_x86_quad_ret(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_ret_t *ret = OFFSETOF_IR_QUAD(quad, ir_quad_ret_t);

	if (ret->src != UINTPTR_MAX)
		SET_EAX(target_x86_util_ebp_offset(ret->src, args));

	LEAVE;
	RET;

	return 0;
}

int target_x86_quad_store(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args)
{
	(void) regs;

	fprintf(stream, "\t#");
	IR_QUAD_FPRINT(stream, quad);

	ir_quad_store_t *store = OFFSETOF_IR_QUAD(quad, ir_quad_store_t);

	SET_EDX(target_x86_util_ebp_offset(store->src, args));
	STORE_RESULT(target_x86_util_ebp_offset(store->dst, args));

	return 0;
}
