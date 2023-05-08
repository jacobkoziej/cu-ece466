// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- shitty intermediate representation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_H
#define JKCC_IR_H


#include <jkcc/ir/bb.h>
#include <jkcc/ir/function.h>
#include <jkcc/ir/ir.h>
#include <jkcc/ir/quad.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/ast.h>


void ir_align_fprint(
	FILE                    *stream,
	size_t                   align);
int ir_declaration(
	ir_context_t            *ir_context,
	ast_t                   *declaration);
ir_static_declaration_t *ir_static_declaration_alloc(
	ir_context_t            *ir_context,
	ast_t                   *declaration);
void ir_reg_fprint(
	FILE                    *stream,
	uintptr_t                reg);
void ir_reg_type_fprint(
	FILE                    *stream,
	ir_reg_type_t            type);
void ir_static_declaration_symbol_fprint(
	FILE                    *stream,
	ir_static_declaration_t *declaration);
ir_unit_t *ir_unit_alloc(
	void);
void ir_unit_deinit(
	ir_unit_t               *ir_unit);
void ir_unit_free(
	ir_unit_t               *ir_unit);
int ir_unit_gen(
	ir_unit_t               *ir_unit,
	ast_t                   *ast);
int ir_unit_init(
	ir_unit_t               *ir_unit);


#endif  /* JKCC_IR_H */
