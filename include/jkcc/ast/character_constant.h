// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * character_constant.h -- character_constant ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_CHARACTER_CONSTANT_H
#define JKCC_AST_CHARACTER_CONSTANT_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/constant.h>
#include <jkcc/location.h>


typedef struct ast_character_constant_s {
	character_constant_t character_constant;
	location_t           location;
	ast_t                ast;
} ast_character_constant_t;


ast_t *ast_character_constant_init(
	character_constant_t *character_constant,
	location_t           *location);
void ast_character_constant_free(
	ast_t                *ast);
void fprint_ast_character_constant(
	FILE                 *stream,
	const ast_t          *ast,
	size_t                level,
	uint_fast8_t          flags);


#endif  /* JKCC_AST_CHARACTER_CONSTANT_H */
