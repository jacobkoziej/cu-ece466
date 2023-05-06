// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- ir base types
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_IR_H
#define JKCC_IR_IR_H


#include <stddef.h>

#include <jkcc/ast/ast.h>
#include <jkcc/ht.h>
#include <jkcc/list.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


typedef enum ir_quad_e {
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

typedef struct ir_unit_s {
	vector_t declaration;  // ast_t*
	vector_t function;     // ir_function_t
} ir_unit_t;

typedef struct ir_context_s {
	ht_t reg;
	struct {
		size_t bb;
		size_t reg;
	} current;
} ir_context_t;


#endif  /* JKCC_IR_IR_H */
