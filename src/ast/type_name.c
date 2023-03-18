// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_name.c -- type_name ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/type_name.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_type_name_init(
	ast_t      *specifier_qualifier_list,
	ast_t      *abstract_declarator,
	location_t *specifier_qualifier_list_location,
	location_t *abstract_declarator_location)
{
	AST_INIT(ast_type_name_t);

	node->specifier_qualifier_list =  specifier_qualifier_list;
	node->abstract_declarator      =  abstract_declarator;

	if (abstract_declarator_location) {
		node->location.file  = specifier_qualifier_list_location->file;
		node->location.start = specifier_qualifier_list_location->start;

		node->location.end   = abstract_declarator_location->end;
	} else {
		node->location = *specifier_qualifier_list_location;
	}

	AST_RETURN(AST_TYPE_NAME);
}

void ast_type_name_free(ast_t *ast)
{
	AST_FREE(ast_type_name_t);

	AST_NODE_FREE(node->specifier_qualifier_list);
	AST_NODE_FREE(node->abstract_declarator);

	free(node);
}

void fprint_ast_type_name(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_name_t);

	INDENT(stream, level);

	fprintf(stream, "\"specifier-qualifier-list\" : ");

	if (node->specifier_qualifier_list)
		FPRINT_AST_NODE(
			stream,
			node->specifier_qualifier_list,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"abstract-declarator\" : ");

	if (node->abstract_declarator)
		FPRINT_AST_NODE(
			stream,
			node->abstract_declarator,
			level + 1,
			AST_PRINT_NO_INDENT_INITIAL |
			AST_PRINT_NO_TRAILING_NEWLINE);
	else
		fprintf(stream, "null");

	fprintf(stream, ",\n");

	FPRINT_AST_NODE_FINISH;
}
