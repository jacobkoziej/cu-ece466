// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- ir base types
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_IR_H
#define JKCC_IR_IR_H


#include <stddef.h>
#include <stdint.h>

#include <jkcc/ast/ast.h>
#include <jkcc/ht.h>
#include <jkcc/list.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


typedef enum ir_quad_e {
	IR_QUAD_BINOP,
	IR_QUAD_TOTAL,
} ir_quad_t;

typedef struct ir_bb_s {
	size_t   id;
	vector_t quad;  // ir_quad_t*
	list_t   list;  // ir_bb_t*
} ir_bb_t;

typedef struct ir_function_s {
	ast_t   *declaration;
	ir_bb_t *bb;
} ir_function_t;

typedef struct ir_static_declaration_s {
	uintptr_t  bb;
	ast_t     *declaration;
} ir_static_declaration_t;

typedef struct ir_unit_s {
	vector_t extern_declaration;  // ast_t*
	vector_t static_declaration;  // ir_static_declaration_t*
	vector_t function;            // ir_function_t
} ir_unit_t;

typedef struct ir_context_s {
	ir_unit_t *ir_unit;
	ht_t       reg;
	ht_t       static_declaration;
	struct {
		size_t    bb;
		uintptr_t reg;
	} current;
} ir_context_t;


#endif  /* JKCC_IR_IR_H */
