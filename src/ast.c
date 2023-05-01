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


void (*const ast_node_free[AST_NODES_TOTAL])(ast_t *ast) = {
	[AST_ADDRESSOF]                = ast_addressof_free,
	[AST_ALIGNAS]                  = ast_alignas_free,
	[AST_ALIGNOF]                  = ast_alignof_free,
	[AST_ARRAY]                    = ast_array_free,
	[AST_ASSIGNMENT]               = ast_assignment_free,
	[AST_ATOMIC]                   = ast_atomic_free,
	[AST_BINARY_OPERATOR]          = ast_binary_operator_free,
	[AST_CALL]                     = ast_call_free,
	[AST_CASE]                     = ast_case_free,
	[AST_CAST]                     = ast_cast_free,
	[AST_CHARACTER_CONSTANT]       = ast_character_constant_free,
	[AST_DECLARATION]              = ast_declaration_free,
	[AST_DEREFERENCE]              = ast_dereference_free,
	[AST_EMPTY]                    = ast_empty_free,
	[AST_EXPRESSION]               = ast_expression_free,
	[AST_FLOATING_CONSTANT]        = ast_floating_constant_free,
	[AST_FOR]                      = ast_for_free,
	[AST_FUNCTION]                 = ast_function_free,
	[AST_FUNCTION_SPECIFIER]       = ast_function_specifier_free,
	[AST_GENERIC_ASSOCIATION]      = ast_generic_association_free,
	[AST_GENERIC_ASSOCIATION_LIST] = ast_generic_association_list_free,
	[AST_GENERIC_SELECTION]        = ast_generic_selection_free,
	[AST_IDENTIFIER]               = ast_identifier_free,
	[AST_IF]                       = ast_if_free,
	[AST_INTEGER_CONSTANT]         = ast_integer_constant_free,
	[AST_LABEL]                    = ast_label_free,
	[AST_LIST]                     = ast_list_free,
	[AST_MEMBER_ACCESS]            = ast_member_access_free,
	[AST_POINTER]                  = ast_pointer_free,
	[AST_SIZEOF]                   = ast_sizeof_free,
	[AST_STATIC_ASSERT]            = ast_static_assert_free,
	[AST_STORAGE_CLASS_SPECIFIER]  = ast_storage_class_specifier_free,
	[AST_STRING_LITERAL]           = ast_string_literal_free,
	[AST_STRUCT]                   = ast_struct_free,
	[AST_SWITCH]                   = ast_switch_free,
	[AST_TERNARY_OPERATOR]         = ast_ternary_operator_free,
	[AST_TYPE]                     = ast_type_free,
	[AST_TYPE_QUALIFIER]           = ast_type_qualifier_free,
	[AST_TYPE_SPECIFIER]           = ast_type_specifier_free,
	[AST_UNARY_OPERATOR]           = ast_unary_operator_free,
	[AST_WHILE]                    = ast_while_free,
};

void (*const fprint_ast_node[AST_NODES_TOTAL])(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags) = {
	[AST_ADDRESSOF]                = fprint_ast_addressof,
	[AST_ALIGNAS]                  = fprint_ast_alignas,
	[AST_ALIGNOF]                  = fprint_ast_alignof,
	[AST_ARRAY]                    = fprint_ast_array,
	[AST_ASSIGNMENT]               = fprint_ast_assignment,
	[AST_ATOMIC]                   = fprint_ast_atomic,
	[AST_BINARY_OPERATOR]          = fprint_ast_binary_operator,
	[AST_CALL]                     = fprint_ast_call,
	[AST_CASE]                     = fprint_ast_case,
	[AST_CAST]                     = fprint_ast_cast,
	[AST_CHARACTER_CONSTANT]       = fprint_ast_character_constant,
	[AST_DECLARATION]              = fprint_ast_declaration,
	[AST_DEREFERENCE]              = fprint_ast_dereference,
	[AST_EMPTY]                    = fprint_ast_empty,
	[AST_EXPRESSION]               = fprint_ast_expression,
	[AST_FLOATING_CONSTANT]        = fprint_ast_floating_constant,
	[AST_FOR]                      = fprint_ast_for,
	[AST_FUNCTION]                 = fprint_ast_function,
	[AST_FUNCTION_SPECIFIER]       = fprint_ast_function_specifier,
	[AST_GENERIC_ASSOCIATION]      = fprint_ast_generic_association,
	[AST_GENERIC_ASSOCIATION_LIST] = fprint_ast_generic_association_list,
	[AST_GENERIC_SELECTION]        = fprint_ast_generic_selection,
	[AST_IDENTIFIER]               = fprint_ast_identifier,
	[AST_IF]                       = fprint_ast_if,
	[AST_INTEGER_CONSTANT]         = fprint_ast_integer_constant,
	[AST_LABEL]                    = fprint_ast_label,
	[AST_LIST]                     = fprint_ast_list,
	[AST_MEMBER_ACCESS]            = fprint_ast_member_access,
	[AST_POINTER]                  = fprint_ast_pointer,
	[AST_SIZEOF]                   = fprint_ast_sizeof,
	[AST_STATIC_ASSERT]            = fprint_ast_static_assert,
	[AST_STORAGE_CLASS_SPECIFIER]  = fprint_ast_storage_class_specifier,
	[AST_STRING_LITERAL]           = fprint_ast_string_literal,
	[AST_STRUCT]                   = fprint_ast_struct,
	[AST_SWITCH]                   = fprint_ast_switch,
	[AST_TERNARY_OPERATOR]         = fprint_ast_ternary_operator,
	[AST_TYPE]                     = fprint_ast_type,
	[AST_TYPE_QUALIFIER]           = fprint_ast_type_qualifier,
	[AST_TYPE_SPECIFIER]           = fprint_ast_type_specifier,
	[AST_UNARY_OPERATOR]           = fprint_ast_unary_operator,
	[AST_WHILE]                    = fprint_ast_while,
};


const char *const ast_node_str[AST_NODES_TOTAL] = {
	[AST_ADDRESSOF]                = "addressof",
	[AST_ALIGNAS]                  = "alignas",
	[AST_ALIGNOF]                  = "alignof",
	[AST_ARRAY]                    = "array",
	[AST_ASSIGNMENT]               = "assignment",
	[AST_ATOMIC]                   = "atomic",
	[AST_BINARY_OPERATOR]          = "binary-operator",
	[AST_CALL]                     = "call",
	[AST_CASE]                     = "case",
	[AST_CAST]                     = "cast-expression",
	[AST_CHARACTER_CONSTANT]       = "character-constant",
	[AST_DECLARATION]              = "declaration",
	[AST_DEREFERENCE]              = "dereference",
	[AST_EMPTY]                    = "empty",
	[AST_EXPRESSION]               = "expression",
	[AST_FLOATING_CONSTANT]        = "floating-constant",
	[AST_FOR]                      = "for",
	[AST_FUNCTION]                 = "function",
	[AST_FUNCTION_SPECIFIER]       = "function-specifier",
	[AST_GENERIC_ASSOCIATION]      = "generic-association",
	[AST_GENERIC_ASSOCIATION_LIST] = "generic-association-list",
	[AST_GENERIC_SELECTION]        = "generic-selection",
	[AST_IDENTIFIER]               = "identifier",
	[AST_IF]                       = "if",
	[AST_INTEGER_CONSTANT]         = "integer-constant",
	[AST_LABEL]                    = "label",
	[AST_LIST]                     = "list",
	[AST_MEMBER_ACCESS]            = "member-access",
	[AST_POINTER]                  = "pointer",
	[AST_SIZEOF]                   = "sizeof",
	[AST_STATIC_ASSERT]            = "static-assert",
	[AST_STORAGE_CLASS_SPECIFIER]  = "storage-class-specifier",
	[AST_STRING_LITERAL]           = "string-literal",
	[AST_STRUCT]                   = "struct",
	[AST_SWITCH]                   = "switch",
	[AST_TERNARY_OPERATOR]         = "ternary-operator",
	[AST_TYPE]                     = "type",
	[AST_TYPE_QUALIFIER]           = "type-qualifier",
	[AST_TYPE_SPECIFIER]           = "type-specifier",
	[AST_UNARY_OPERATOR]           = "unary-operator",
	[AST_WHILE]                    = "while",
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
