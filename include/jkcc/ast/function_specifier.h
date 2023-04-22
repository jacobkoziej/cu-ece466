// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * function_specifier.h -- function_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_FUNCTION_SPECIFIER_H
#define JKCC_AST_FUNCTION_SPECIFIER_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_FUNCTION_SPECIFIER_INLINE    (1 << 0)
#define AST_FUNCTION_SPECIFIER__NORETURN (1 << 1)


typedef struct ast_function_specifier_s {
	uint_fast8_t specifier;
	location_t   location;
	ast_t        ast;
} ast_function_specifier_t;


ast_t *ast_function_specifier_init(
	uint_fast8_t  specifier,
	location_t   *location);
void ast_function_specifier_free(
	ast_t        *ast);
void fprint_ast_function_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_FUNCTION_SPECIFIER_H */
