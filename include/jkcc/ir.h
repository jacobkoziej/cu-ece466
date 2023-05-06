// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_H
#define JKCC_IR_H


#include <jkcc/ir/ir.h>

#include <jkcc/ast.h>


#define IR_ERROR_NOMEM                  (-1)
#define IR_ERROR_EMPTY_TRANSLATION_UNIT (-2)
#define IR_ERROR_UNKNOWN_AST_NODE       (-3)


ir_unit_t *ir_unit_alloc(
	void);
void ir_unit_deinit(
	ir_unit_t *ir_unit);
void ir_unit_free(
	ir_unit_t *ir_unit);
int ir_unit_gen(
	ir_unit_t *ir_unit,
	ast_t     *ast);
int ir_unit_init(
	ir_unit_t *ir_unit);


#endif  /* JKCC_IR_H */
