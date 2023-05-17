// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * quad.c -- x86 target code generation for quads
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/x86/quad.h>

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
	[IR_QUAD_BINOP]  = NULL,
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
