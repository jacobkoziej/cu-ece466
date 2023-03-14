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
#include <string.h>

#include <jkcc/lexer.h>
#include <jkcc/parser.h>


#define INDENT(stream, level) if (level) {              \
	char buf[level];                                \
	memset(buf, '\t', sizeof(buf));                 \
	fwrite(buf, sizeof(*buf), sizeof(buf), stream); \
}

#define OFFSETOF_AST_NODE(node, type) ((type*) (((uintptr_t) node) - offsetof(type, ast)))


static void fprint_ast_identifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags);
static void fprint_file(
	FILE             *stream,
	const file_t     *file,
	size_t            level,
	uint_fast8_t      flags);
static void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags);


#endif  /* JKCC_PRIVATE_AST_H */
