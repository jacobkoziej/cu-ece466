// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * storage_class_specifier.h -- storage_class_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_STORAGE_CLASS_SPECIFIER_H
#define JKCC_AST_STORAGE_CLASS_SPECIFIER_H


#include <jkcc/ast/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_STORAGE_CLASS_SPECIFIER_TYPEDEF       (1 << 0)
#define AST_STORAGE_CLASS_SPECIFIER_EXTERN        (1 << 1)
#define AST_STORAGE_CLASS_SPECIFIER_STATIC        (1 << 2)
#define AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL (1 << 3)
#define AST_STORAGE_CLASS_SPECIFIER_AUTO          (1 << 4)
#define AST_STORAGE_CLASS_SPECIFIER_REGISTER      (1 << 5)


typedef struct ast_storage_class_specifier_s {
	uint_fast8_t specifier;
	location_t   location;
	ast_t        ast;
} ast_storage_class_specifier_t;


ast_t *ast_storage_class_specifier_init(
	uint_fast8_t  specifier,
	location_t   *location);
void ast_storage_class_specifier_free(
	ast_t        *ast);
void fprint_ast_storage_class_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_STORAGE_CLASS_SPECIFIER_H */
