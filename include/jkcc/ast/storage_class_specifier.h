// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * storage_class_specifier.h -- storage_class_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_STORAGE_CLASS_SPECIFIER_H
#define JKCC_AST_STORAGE_CLASS_SPECIFIER_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


typedef struct ast_storage_class_specifier_s {
	int        type;
	location_t location;
	ast_t      ast;
} ast_storage_class_specifier_t;


ast_t *ast_storage_class_specifier_init(
	int           type,
	location_t   *location);
void ast_storage_class_specifier_free(
	ast_t        *ast);
void fprint_ast_storage_class_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_STORAGE_CLASS_SPECIFIER_H */
