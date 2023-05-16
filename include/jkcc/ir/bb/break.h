// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * break.h -- break basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_BB_BREAK_H
#define JKCC_IR_BB_BREAK_H


#include <jkcc/ir/ir.h>

#include <jkcc/ast.h>


int ir_bb_break_gen(
	ir_context_t *ir_context,
	ast_t        *ast);


#endif  /* JKCC_IR_BB_BREAK_H */
