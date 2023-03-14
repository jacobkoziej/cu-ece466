// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.h -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_H
#define JKCC_AST_H


#include <jkcc/parser.h>
#include <jkcc/string.h>


#define AST_PRINT_NO_INDENT_INITIAL   (1 << 0)
#define AST_PRINT_NO_TRAILING_NEWLINE (1 << 1)


typedef enum ast_e {
	AST_IDENTIFIER,
	AST_NODES_TOTAL,
} ast_t;


typedef struct ast_identifier_s {
	string_t   identifier;
	location_t location;
	ast_t      ast;
} ast_identifier_t;


ast_t *ast_identifier_init(string_t *identifier, location_t *location);


#endif  /* JKCC_AST_H */
