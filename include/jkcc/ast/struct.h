// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * struct.h -- struct ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_STRUCT_H
#define JKCC_AST_STRUCT_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>
#include <jkcc/symbol.h>


#define AST_STRUCT_STRUCT (1 << 0)
#define AST_STRUCT_UNION  (1 << 1)


typedef struct ast_struct_s {
	ast_t          *tag;
	ast_t          *declaration_list;
	symbol_table_t *members;
	uint_fast8_t    type;
	location_t      location;
	ast_t           ast;
} ast_struct_t;


ast_t *ast_struct_init(
	ast_t          *tag,
	ast_t          *declaration_list,
	symbol_table_t *members,
	uint_fast8_t    type,
	location_t     *location_start,
	location_t     *location_end);
void ast_struct_free(
	ast_t          *ast);
ast_t *ast_struct_get_declaration_list(
	ast_t          *ast);
void ast_struct_set_declaration_list(
	ast_t          *ast,
	ast_t          *declaration_list);
void ast_struct_set_symbol_table(
	ast_t          *ast,
	symbol_table_t *members);
void fprint_ast_struct(
	FILE           *stream,
	const ast_t    *ast,
	size_t          level,
	uint_fast8_t    flags);


#endif  /* JKCC_AST_STRUCT_H */
