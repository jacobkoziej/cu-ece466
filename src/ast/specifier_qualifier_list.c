// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * specifier_qualifier_list.c -- specifier_qualifier_list ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/specifier_qualifier_list.h>
#include <jkcc/ast/type_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/lexer.h>
#include <jkcc/vector.h>


#define TYPE_SPECIFIER_SHORT_INT ( \
	TYPE_SPECIFIER_SHORT |     \
	TYPE_SPECIFIER_INT)        \

#define TYPE_SPECIFIER_LONG_INT ( \
	TYPE_SPECIFIER_LONG |     \
	TYPE_SPECIFIER_INT)

#define TYPE_SPECIFIER_LONG_LONG_INT ( \
	TYPE_SPECIFIER_LONG_LONG |     \
	TYPE_SPECIFIER_LONG |          \
	TYPE_SPECIFIER_INT)

#define TYPE_SPECIFIER_DOUBLE__COMPLEX ( \
	TYPE_SPECIFIER_DOUBLE |          \
	TYPE_SPECIFIER__COMPLEX)

#define TYPE_SPECIFIER_LONG_DOUBLE__COMPLEX ( \
	TYPE_SPECIFIER_LONG |                 \
	TYPE_SPECIFIER_DOUBLE |               \
	TYPE_SPECIFIER__COMPLEX)


ast_t *ast_specifier_qualifier_list_append(
	ast_t         *specifier_qualifier_list,
	ast_t         *type_specifier,
	ast_t         *type_qualifier,
	location_t    *location,
	const char   **error)
{
	ast_specifier_qualifier_list_t *list = (specifier_qualifier_list)
		? OFFSETOF_AST_NODE(
			specifier_qualifier_list,
			ast_specifier_qualifier_list_t)
		: NULL;

	ast_type_specifier_t *specifier = (type_specifier)
		? OFFSETOF_AST_NODE(type_specifier, ast_type_specifier_t)
		: NULL;

	if (list && specifier) {
		uint16_t contraints = list->specifier;

		if (
			!(specifier->specifier &
			(TYPE_SPECIFIER_SIGNED | TYPE_SPECIFIER_UNSIGNED)))
			contraints &= ~(TYPE_SPECIFIER_SIGNED |
					TYPE_SPECIFIER_UNSIGNED);

		switch (specifier->specifier) {
			case TYPE_SPECIFIER_VOID:
				if (contraints)
					goto multiset_error;

				// void
				break;

			case TYPE_SPECIFIER_CHAR:
				if (contraints)
					goto multiset_error;

				// char
				break;

			case TYPE_SPECIFIER_SHORT:
				// short int
				if (contraints & ~TYPE_SPECIFIER_INT)
					goto multiset_error;

				// short
				break;

			case TYPE_SPECIFIER_INT:
				if (contraints)
					goto multiset_error;

				// int
				break;

			case TYPE_SPECIFIER_LONG:
				if (
					contraints &
					~(TYPE_SPECIFIER_LONG_INT |
						TYPE_SPECIFIER_DOUBLE__COMPLEX))
					goto multiset_error;

				// long (long (int) | double (_Complex))
				break;

			case TYPE_SPECIFIER_FLOAT:
				if (contraints & ~TYPE_SPECIFIER__COMPLEX)
					goto multiset_error;

				// float (_Complex)
				break;

			case TYPE_SPECIFIER_DOUBLE:
				if (contraints & ~TYPE_SPECIFIER__COMPLEX)
					goto multiset_error;

				// double (_Complex)
				break;

			case TYPE_SPECIFIER_SIGNED:
			case TYPE_SPECIFIER_UNSIGNED:
				if (
					contraints &
					~(TYPE_SPECIFIER_CHAR |
						TYPE_SPECIFIER_LONG_LONG_INT |
						TYPE_SPECIFIER_SHORT_INT))
					goto multiset_error;

				// [signed | unsigned] (short | long | long long) (int)
				break;

			case TYPE_SPECIFIER__BOOL:
				if (contraints)
					goto multiset_error;

				// _Bool
				break;

			case TYPE_SPECIFIER__COMPLEX:
				if (contraints)
					goto multiset_error;

				// _Complex
				break;

			default:
				*error = "unkown type";
				return NULL;
		}
	}

	if (type_specifier) {
		if (vector_append(&list->type_specifier, &type_specifier))
			goto vector_error;

		list->specifier |= specifier->specifier;
	}

	if (type_qualifier)
		if (vector_append(&list->type_qualifier, &type_qualifier))
			goto vector_error;

	list->location.start = location->start;

	return specifier_qualifier_list;

vector_error:
	return NULL;

multiset_error:
	*error = "invalid type multiset";

	return NULL;
}

ast_t *ast_specifier_qualifier_list_init(
	ast_t         *type_specifier,
	ast_t         *type_qualifier,
	location_t    *location)
{
	AST_INIT(ast_specifier_qualifier_list_t);

	memset(node, 0, sizeof(*node));

	if (vector_init(&node->type_specifier, sizeof(type_specifier), 0))
		goto error_init;

	if (vector_init(&node->type_qualifier, sizeof(type_qualifier), 0))
		goto error_init;

	if (type_specifier) {
		if (vector_append(&node->type_specifier, &type_specifier))
			goto error_append;

		ast_type_specifier_t *specifier = OFFSETOF_AST_NODE(
			type_specifier,
			ast_type_specifier_t);

		node->specifier = specifier->specifier;
	}

	if (type_qualifier)
		if (vector_append(&node->type_qualifier, &type_qualifier))
			goto error_append;

	node->location = *location;

	AST_RETURN(AST_SPECIFIER_QUALIFIER_LIST);

error_append:
	vector_free(&node->type_specifier);
	vector_free(&node->type_qualifier);

error_init:
	return NULL;
}

void ast_specifier_qualifier_list_free(ast_t *ast)
{
	AST_FREE(ast_specifier_qualifier_list_t);

	ast_t **type_specifier = node->type_specifier.buf;
	ast_t **type_qualifier = node->type_qualifier.buf;

	for (size_t i = 0; i < node->type_qualifier.use; i++)
		AST_NODE_FREE(type_qualifier[i]);

	for (size_t i = 0; i < node->type_specifier.use; i++)
		AST_NODE_FREE(type_specifier[i]);

	vector_free(&node->type_specifier);
	vector_free(&node->type_qualifier);

	free(node);
}

void fprint_ast_specifier_qualifier_list(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_specifier_qualifier_list_t);

	if (node->type_specifier.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_TYPE_SPECIFIER],
			node->type_specifier);

	if (node->type_qualifier.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_TYPE_QUALIFIER],
			node->type_qualifier);

	FPRINT_AST_NODE_FINISH;
}
