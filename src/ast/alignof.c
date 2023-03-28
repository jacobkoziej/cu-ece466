// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * alignof.c -- alignof ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/alignof.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_alignof_init(
	ast_t      *operand,
	location_t *location_start,
	location_t *location_end)
{
	AST_INIT(ast_alignof_t);

	node->operand = operand;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_ALIGNOF);
}

void ast_alignof_free(ast_t *ast)
{
	AST_FREE(ast_alignof_t);

	AST_NODE_FREE(node->operand);

	free(node);
}

void fprint_ast_alignof(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_alignof_t);

	INDENT(stream, level);
	fprintf(stream, "\"operand\" : ");
	FPRINT_AST_NODE(
		stream,
		node->operand,
		level,
		AST_PRINT_NO_INDENT_INITIAL |
		AST_PRINT_NO_TRAILING_NEWLINE);
	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
