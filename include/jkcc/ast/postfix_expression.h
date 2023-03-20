// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * postfix_expression.h -- postfix_expression ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_POSTFIX_EXPRESSION_H
#define JKCC_AST_POSTFIX_EXPRESSION_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>


#define POSTFIX_EXPRESSION_DEREFERENCE (1 << 0)
#define POSTFIX_EXPRESSION_PARENTHESIS (1 << 1)
#define POSTFIX_EXPRESSION_INCREMENT   (1 << 2)
#define POSTFIX_EXPRESSION_DECREMENT   (1 << 3)


typedef struct ast_postfix_expression_s {
	ast_t        *postfix_expression;
	ast_t        *expression;
	ast_t        *argument_expression_list;
	ast_t        *identifier;
	ast_t        *type_name;
	ast_t        *initializer_list;
	uint_fast8_t  flags;
	location_t    location;
	ast_t         ast;
} ast_postfix_expression_t;


ast_t *ast_postfix_expression_init(
	ast_t        *postfix_expression,
	ast_t        *expression,
	ast_t        *argument_expression_list,
	ast_t        *identifier,
	ast_t        *type_name,
	ast_t        *initializer_list,
	uint_fast8_t  flags,
	location_t   *location_start,
	location_t   *location_end);
void ast_postfix_expression_free(
	ast_t        *ast);
void fprint_ast_postfix_expression(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);


#endif  /* JKCC_AST_POSTFIX_EXPRESSION_H */
