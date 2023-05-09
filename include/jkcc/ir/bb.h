// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * bb.h -- basic block ir
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_BB_H
#define JKCC_IR_BB_H


#include <jkcc/ir/ir.h>

#include <jkcc/ir/bb/declaration.h>
#include <jkcc/ir/bb/mov.h>
#include <jkcc/ir/bb/statement.h>

#include <jkcc/ast.h>


#define IR_BB_GEN(ir_context, ast) ir_bb_gen[*ast](ir_context, ast)


extern int (*const ir_bb_gen[AST_NODES_TOTAL])(
	ir_context_t *ir_context,
	ast_t        *ast);


ir_bb_t *ir_bb_alloc(
	size_t        id);
void ir_bb_free(
	ir_bb_t      *ir_bb);
int ir_bb_unknown_gen(
	ir_context_t *ir_context,
	ast_t        *ast);


#endif  /* JKCC_IR_BB_H */
