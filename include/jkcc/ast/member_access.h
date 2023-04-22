// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * member_access.h -- member_access ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_MEMBER_ACCESS_H
#define JKCC_AST_MEMBER_ACCESS_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/location.h>


typedef struct ast_member_access_s {
	ast_t      *operand;
	ast_t      *identifier;
	location_t  location;
	ast_t       ast;
} ast_member_access_t;


ast_t *ast_member_access_init(
	ast_t        *operand,
	ast_t        *identifier,
	location_t   *location_start,
	location_t   *location_end);
void ast_member_access_free(
	ast_t        *ast);
void fprint_ast_member_access(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_MEMBER_ACCESS_H */
