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

#include <jkcc/parser.h>


#define INDENT(stream, level) if (level) {              \
	char buf[level];                                \
	memset(buf, '\t', sizeof(buf));                 \
	fwrite(buf, sizeof(*buf), sizeof(buf), stream); \
}


static void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags);


#endif  /* JKCC_PRIVATE_AST_H */
