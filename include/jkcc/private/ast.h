// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.h -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_AST_H
#define JKCC_PRIVATE_AST_H


#include <jkcc/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/lexer.h>


#define INDENT(stream, level) if (level) {              \
	char buf[level];                                \
	memset(buf, '\t', sizeof(buf));                 \
	fwrite(buf, sizeof(*buf), sizeof(buf), stream); \
}

#define OFFSETOF_AST_NODE(node, type) ((type*) (((uintptr_t) node) - offsetof(type, ast)))

#define AST_INIT(type)                      \
	type *node = malloc(sizeof(*node)); \
	if (!node) return NULL;

#define AST_RETURN(val)    \
	node->ast = val;   \
	return &node->ast;

#define AST_FREE(type)                             \
	if (!ast) return;                          \
	type *node = OFFSETOF_AST_NODE(ast, type);

#define FPRINT_AST_BEGIN                            \
	if (!(flags & AST_PRINT_NO_INDENT_INITIAL)) \
		INDENT(stream, level);              \
                                                    \
	fprintf(stream, "{\n");                     \
                                                    \
	++level;

#define FPRINT_AST_NODE_BEGIN(type)                 \
	type *node = OFFSETOF_AST_NODE(ast, type);  \
                                                    \
	FPRINT_AST_BEGIN;

#define FPRINT_AST_FINISH \
	--level;                                      \
                                                      \
	INDENT(stream, level);                        \
	fprintf(stream, "}");                         \
                                                      \
	if (!(flags & AST_PRINT_NO_TRAILING_NEWLINE)) \
		fprintf(stream, "\n");


#endif  /* JKCC_PRIVATE_AST_H */
