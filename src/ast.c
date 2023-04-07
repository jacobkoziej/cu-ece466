// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.c -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <jkcc/list.h>
#include <jkcc/location.h>
#include <jkcc/parser.h>
#include <jkcc/string.h>


void (*ast_node_free[AST_NODES_TOTAL])(ast_t *ast) = {
	ast_addressof_free,
	ast_alignas_free,
	ast_alignof_free,
	ast_assignment_free,
	ast_atomic_free,
	ast_binary_operator_free,
	ast_cast_free,
	ast_character_constant_free,
	ast_dereference_free,
	ast_expression_free,
	ast_floating_constant_free,
	ast_function_specifier_free,
	ast_generic_association_free,
	ast_generic_association_list_free,
	ast_generic_selection_free,
	ast_identifier_free,
	ast_init_declarator_list_free,
	ast_integer_constant_free,
	ast_member_access_free,
	ast_pointer_free,
	ast_sizeof_free,
	ast_static_assert_free,
	ast_storage_class_specifier_free,
	ast_string_literal_free,
	ast_ternary_operator_free,
	ast_type_free,
	ast_type_name_free,
	ast_type_qualifier_free,
	ast_type_qualifier_list_free,
	ast_type_specifier_free,
	ast_unary_operator_free,
};

void (*fprint_ast_node[AST_NODES_TOTAL])(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags) = {
	fprint_ast_addressof,
	fprint_ast_alignas,
	fprint_ast_alignof,
	fprint_ast_assignment,
	fprint_ast_atomic,
	fprint_ast_binary_operator,
	fprint_ast_cast,
	fprint_ast_character_constant,
	fprint_ast_dereference,
	fprint_ast_expression,
	fprint_ast_floating_constant,
	fprint_ast_function_specifier,
	fprint_ast_generic_association,
	fprint_ast_generic_association_list,
	fprint_ast_generic_selection,
	fprint_ast_identifier,
	fprint_ast_init_declarator_list,
	fprint_ast_integer_constant,
	fprint_ast_member_access,
	fprint_ast_pointer,
	fprint_ast_sizeof,
	fprint_ast_static_assert,
	fprint_ast_storage_class_specifier,
	fprint_ast_string_literal,
	fprint_ast_ternary_operator,
	fprint_ast_type,
	fprint_ast_type_name,
	fprint_ast_type_qualifier,
	fprint_ast_type_qualifier_list,
	fprint_ast_type_specifier,
	fprint_ast_unary_operator,
};


char *ast_node_str[AST_NODES_TOTAL] = {
	"addressof",
	"alignas",
	"alignof",
	"assignment",
	"atomic",
	"binary-operator",
	"cast-expression",
	"character-constant",
	"dereference",
	"expression",
	"floating-constant",
	"function-specifier",
	"generic-association",
	"generic-association-list",
	"generic-selection",
	"identifier",
	"init-declarator-list",
	"integer-constant",
	"member-access",
	"pointer",
	"sizeof",
	"static-assert",
	"storage-class-specifier",
	"string-literal",
	"ternary-operator",
	"type",
	"type-name",
	"type-qualifier",
	"type-qualifier-list",
	"type-specifier",
	"unary-operator",
};


void fprint_file(
	FILE             *stream,
	const file_t     *file,
	size_t            level,
	uint_fast8_t      flags)
{
	FPRINT_AST_BEGIN;

	INDENT(stream, level);
	fprintf(stream, "\"path\" : [\n");

	++level;

	const list_t *tail = &file->list;
	const file_t *cur  = file;

	while (tail->prev) {
		cur = OFFSETOF_LIST(tail, file_t, list);

		INDENT(stream, level);
		fprintf(stream, "\"%s\",\n", cur->path);

		tail = tail->prev;
	}

	INDENT(stream, level);
	fprintf(stream, "\"%s\"\n", cur->path);

	--level;

	INDENT(stream, level);
	fprintf(stream, "],\n");

	INDENT(stream, level);
	fprintf(stream, "\"refs\" : %lu\n", file->refs);

	FPRINT_AST_FINISH;
}

void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags)
{
	FPRINT_AST_BEGIN;

	INDENT(stream, level);
	fprintf(stream, "\"file\" : ");
	fprint_file(
		stream,
		location->file,
		level,
		AST_PRINT_NO_INDENT_INITIAL | AST_PRINT_NO_TRAILING_NEWLINE);
	fprintf(stream, ",\n");

	INDENT(stream, level);
	fprintf(stream, "\"start\" : {\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"offset\" : %ld,\n", location->start.offset);
	INDENT(stream, level);
	fprintf(stream, "\"line\"   : %d,\n", location->start.line);
	INDENT(stream, level);
	fprintf(stream, "\"column\" : %d\n", location->start.column);

	--level;

	INDENT(stream, level);
	fprintf(stream, "},\n");

	INDENT(stream, level);
	fprintf(stream, "\"end\" : {\n");

	++level;

	INDENT(stream, level);
	fprintf(stream, "\"offset\" : %ld,\n", location->end.offset);
	INDENT(stream, level);
	fprintf(stream, "\"line\"   : %d,\n", location->end.line);
	INDENT(stream, level);
	fprintf(stream, "\"column\" : %d\n", location->end.column);

	--level;

	INDENT(stream, level);
	fprintf(stream, "}\n");

	FPRINT_AST_FINISH;
}
