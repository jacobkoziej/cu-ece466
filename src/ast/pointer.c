// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pointer.c -- pointer ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/pointer.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_pointer_init(
	ast_t      *type_qualifier_list,
	ast_t      *pointer,
	location_t *location)
{
	AST_INIT(ast_pointer_t);

	node->type_qualifier_list =  type_qualifier_list;
	node->pointer             =  pointer;
	node->location            = *location;

	AST_RETURN(AST_POINTER);
}

void ast_pointer_free(ast_t *ast)
{
	AST_FREE(ast_pointer_t);

	AST_NODE_FREE(node->type_qualifier_list);
	AST_NODE_FREE(node->pointer);

	free(node);
}

void fprint_ast_pointer(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_pointer_t);

	INDENT(stream, level);

	fprintf(stream, "\"type-qualifier-list\" : ");

	if (node->type_qualifier_list)
		fprint_ast_type_qualifier_list(
			stream,
			node->type_qualifier_list,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"pointer\" : ");

	if (node->pointer)
		fprint_ast_pointer(
			stream,
			node->pointer,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
