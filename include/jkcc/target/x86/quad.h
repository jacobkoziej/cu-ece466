// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * quad.h -- x86 target code generation for quads
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_TARGET_X86_QUAD_H
#define JKCC_TARGET_X86_QUAD_H


#include <stdint.h>
#include <stdio.h>

#include <jkcc/ir.h>


#define TARGET_X86_QUAD(stream, quad, regs, args) target_x86_quad[*quad](stream, quad, regs, args)


extern int (*const target_x86_quad[IR_QUAD_TOTAL])(
	FILE      *stream,
	ir_quad_t *quad,
	uintptr_t  regs,
	uintptr_t  args);


#endif  /* JKCC_TARGET_X86_QUAD_H */
