// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ir.h -- ir base types
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_IR_IR_H
#define JKCC_IR_IR_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <jkcc/ast/ast.h>
#include <jkcc/ht.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


#define IR_ERROR_NOMEM                       (-1)
#define IR_ERROR_EMPTY_TRANSLATION_UNIT      (-2)
#define IR_ERROR_UNKNOWN_AST_NODE            (-3)
#define IR_ERROR_UNIMPLEMENTED_STORAGE_CLASS (-4)
#define IR_ERROR_EMPTY_FUNCTION_BODY         (-5)


typedef enum ir_reg_type_e {
	IR_REG_TYPE_I32,
	IR_REG_TYPE_PTR,
} ir_reg_type_t;

typedef enum ir_quad_e {
	IR_QUAD_ALLOCA,
	IR_QUAD_ARG,
	IR_QUAD_BINOP,
	IR_QUAD_BR,
	IR_QUAD_CALL,
	IR_QUAD_CMP,
	IR_QUAD_LOAD,
	IR_QUAD_MOV,
	IR_QUAD_STORE,
	IR_QUAD_TOTAL,
} ir_quad_t;

typedef struct ir_bb_s {
	size_t   id;
	vector_t quad;  // ir_quad_t*
} ir_bb_t;

typedef struct ir_function_s {
	ir_reg_type_t  return_type;
	vector_t      *argv;         // ast_t*
	ast_t         *declaration;
	vector_t       bb;           // ir_bb_t*
	struct {
		ht_t lookup;
		ht_t type;
	} reg;
} ir_function_t;

typedef struct ir_static_declaration_s {
	uintptr_t  bb;
	ast_t     *declaration;
} ir_static_declaration_t;

typedef enum ir_location_type_e {
	IR_LOCATION_REG,
	IR_LOCATION_EXTERN_DECLARATION,
	IR_LOCATION_STATIC_DECLARATION,
} ir_location_type_t;

typedef struct ir_location_s {
	ir_location_type_t type;
	union {
		uintptr_t                reg;
		ast_t                   *extern_declaration;
		ir_static_declaration_t *static_declaration;
	};
} ir_location_t;

typedef struct ir_unit_s {
	vector_t extern_declaration;  // ast_t*
	vector_t static_declaration;  // ir_static_declaration_t*
	vector_t function;            // ir_function_t*
} ir_unit_t;

typedef struct ir_context_s {
	ir_unit_t     *ir_unit;
	ir_function_t *ir_function;
	ir_bb_t       *ir_bb;
	ht_t           static_declaration;
	uintptr_t      result;
	bool           lvalue;
	struct {
		size_t    bb;
		uintptr_t dst;
	} current;
} ir_context_t;


#endif  /* JKCC_IR_IR_H */
