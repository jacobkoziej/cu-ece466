// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * declaration.h -- declaration ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_DECLARATION_H
#define JKCC_AST_DECLARATION_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_DECLARATION_IMPLICIT_EXTERN (1 << 0)
#define AST_DECLARATION_EXPLICIT_EXTERN (1 << 1)
#define AST_DECLARATION_STATIC          (1 << 2)
#define AST_DECLARATION_AUTO            (1 << 3)
#define AST_DECLARATION_REGISTER        (1 << 4)
#define AST_DECLARATION_ARGUMENT        (1 << 5)


typedef struct ast_declaration_s {
	ast_t         *type;
	ast_t         *identifier;
	ast_t         *initializer;
	uint_fast8_t   storage_class;
	location_t     location;
	ast_t          ast;
} ast_declaration_t;


ast_t *ast_declaration_init(
	ast_t         *type,
	ast_t         *identifier,
	ast_t         *initializer,
	uint_fast8_t   storage_class,
	location_t    *location_start,
	location_t    *location_end);
void ast_declaration_free(
	ast_t         *ast);
ast_t *ast_declaration_get_identifier(
	ast_t         *ast);
uint_fast8_t ast_declaration_get_storage_class(
	ast_t         *ast);
ast_t *ast_declaration_get_type(
	ast_t         *ast);
void fprint_ast_declaration(
	FILE          *stream,
	const ast_t   *ast,
	size_t         level,
	uint_fast8_t   flags);


#endif  /* JKCC_AST_DECLARATION_H */
