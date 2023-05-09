// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * bb.h -- basic block ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_BB_H
#define JKCC_IR_BB_H


#include <jkcc/ir/ir.h>

#include <jkcc/ir/bb/statement.h>


ir_bb_t *ir_bb_alloc(
	size_t   id);
void ir_bb_free(
	ir_bb_t *ir_bb);


#endif  /* JKCC_IR_BB_H */
