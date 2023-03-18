// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_specifier.h -- type_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_TYPE_SPECIFIER_H
#define JKCC_AST_TYPE_SPECIFIER_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define TYPE_SPECIFIER_VOID                      (1 << 0)
#define TYPE_SPECIFIER_CHAR                      (1 << 1)
#define TYPE_SPECIFIER_SHORT                     (1 << 2)
#define TYPE_SPECIFIER_INT                       (1 << 3)
#define TYPE_SPECIFIER_LONG                      (1 << 4)
#define TYPE_SPECIFIER_LONG_LONG                 (1 << 5)   // multiset for ast_specifier_qualifier_list_t
#define TYPE_SPECIFIER_FLOAT                     (1 << 6)
#define TYPE_SPECIFIER_DOUBLE                    (1 << 7)
#define TYPE_SPECIFIER_SIGNED                    (1 << 8)
#define TYPE_SPECIFIER_UNSIGNED                  (1 << 9)
#define TYPE_SPECIFIER__BOOL                     (1 << 10)
#define TYPE_SPECIFIER__COMPLEX                  (1 << 11)
#define TYPE_SPECIFIER_ATOMIC_TYPE_SPECIFIER     (1 << 12)
#define TYPE_SPECIFIER_STRUCT_OR_UNION_SPECIFIER (1 << 13)
#define TYPE_SPECIFIER_ENUM_SPECIFIER            (1 << 14)
#define TYPE_SPECIFIER_TYPEDEF_NAME              (1 << 15)


typedef struct ast_type_specifier_s {
	uint16_t    type;
	ast_t      *semantic_type;
	location_t  location;
	ast_t       ast;
} ast_type_specifier_t;


ast_t *ast_type_specifier_init(
	int           keyword_type,
	ast_t        *semantic_type,
	location_t   *location);
void ast_type_specifier_free(
	ast_t        *ast);
void fprint_ast_type_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_TYPE_SPECIFIER_H */
