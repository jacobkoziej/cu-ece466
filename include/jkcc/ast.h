// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.h -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_H
#define JKCC_AST_H


#include <stdint.h>
#include <stdio.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>


#define AST_PRINT_NO_INDENT_INITIAL   (1 << 0)
#define AST_PRINT_NO_TRAILING_NEWLINE (1 << 1)

#define AST_NODE_FREE(ast) if (ast) ast_node_free[*ast](ast);

#define FPRINT_AST_NODE(stream, ast, level, flags) fprint_ast_node[*ast]( \
	stream,                                                           \
	ast,                                                              \
	level,                                                            \
	flags)


typedef enum ast_e {
	AST_IDENTIFIER,
	AST_NODES_TOTAL,
} ast_t;


typedef struct ast_identifier_s {
	identifier_t identifier;
	location_t   location;
	ast_t        ast;
} ast_identifier_t;


extern void (*ast_node_free[AST_NODES_TOTAL])(ast_t *ast);

extern void (*fprint_ast_node[AST_NODES_TOTAL])(
	FILE         *stream,
	const ast_t  *ast,
	size_t       level,
	uint_fast8_t flags);


ast_t *ast_identifier_init(identifier_t *identifier, location_t *location);
void   ast_identifier_free(ast_t *ast);


#endif  /* JKCC_AST_H */
