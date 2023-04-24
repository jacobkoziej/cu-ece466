// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type.c -- type ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/type.h>
#include <jkcc/ast/type_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/location.h>
#include <jkcc/vector.h>


#define AST_TYPE_SPECIFIER_SHORT_INT ( \
	AST_TYPE_SPECIFIER_SHORT |     \
	AST_TYPE_SPECIFIER_INT)        \

#define AST_TYPE_SPECIFIER_LONG_INT ( \
	AST_TYPE_SPECIFIER_LONG |     \
	AST_TYPE_SPECIFIER_INT)

#define AST_TYPE_SPECIFIER_LONG_LONG_INT ( \
	AST_TYPE_SPECIFIER_LONG_LONG |     \
	AST_TYPE_SPECIFIER_LONG |          \
	AST_TYPE_SPECIFIER_INT)

#define AST_TYPE_SPECIFIER_DOUBLE__COMPLEX ( \
	AST_TYPE_SPECIFIER_DOUBLE |          \
	AST_TYPE_SPECIFIER__COMPLEX)

#define AST_TYPE_SPECIFIER_LONG_DOUBLE__COMPLEX ( \
	AST_TYPE_SPECIFIER_LONG |                 \
	AST_TYPE_SPECIFIER_DOUBLE |               \
	AST_TYPE_SPECIFIER__COMPLEX)


ast_t *ast_type_append(
	ast_t         *type,
	ast_t         *specifier,
	location_t    *location,
	const char   **error)
{
	ast_type_t *list = OFFSETOF_AST_NODE(type, ast_type_t);

	ast_storage_class_specifier_t *storage_class_specifier =
		(*specifier == AST_STORAGE_CLASS_SPECIFIER)
		? OFFSETOF_AST_NODE(specifier, ast_storage_class_specifier_t)
		: NULL;

	ast_type_specifier_t *type_specifier =
		(*specifier == AST_TYPE_SPECIFIER)
		? OFFSETOF_AST_NODE(specifier, ast_type_specifier_t)
		: NULL;

	ast_type_qualifier_t *type_qualifier =
		(*specifier == AST_TYPE_QUALIFIER)
		? OFFSETOF_AST_NODE(specifier, ast_type_qualifier_t)
		: NULL;

	ast_function_specifier_t *function_specifier =
		(*specifier == AST_FUNCTION_SPECIFIER)
		? OFFSETOF_AST_NODE(specifier, ast_function_specifier_t)
		: NULL;

	ast_alignas_t *alignment_specifier =
		(*specifier == AST_ALIGNAS)
		? OFFSETOF_AST_NODE(specifier, ast_alignas_t)
		: NULL;

	if (storage_class_specifier) {
		uint_fast8_t constraints = list->storage_class_specifier;

		if (constraints & ~AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL)
			if (storage_class_specifier->specifier !=
				AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL)
				goto multiple_storage_class_error;

		if (constraints & storage_class_specifier->specifier)
			goto duplicate_storage_class_error;

		if (
			storage_class_specifier->specifier ==
			AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL)
			if (
				constraints &
				~(AST_STORAGE_CLASS_SPECIFIER_EXTERN |
					AST_STORAGE_CLASS_SPECIFIER_STATIC))
				goto thread_local_error;

		if (constraints & AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL)
			if (
				storage_class_specifier->specifier &
				~(AST_STORAGE_CLASS_SPECIFIER_EXTERN |
					AST_STORAGE_CLASS_SPECIFIER_STATIC))
				goto thread_local_error;

		if (vector_append(
			&list->storage_class_specifier_vector,
			&specifier)) goto vector_error;

		list->storage_class_specifier |=
			storage_class_specifier->specifier;
	}

	if (type_specifier) {
		uint_fast16_t constraints = list->type_specifier;

		if (
			!(type_specifier->specifier &
			(AST_TYPE_SPECIFIER_SIGNED |
				AST_TYPE_SPECIFIER_UNSIGNED)))
			constraints &= ~(AST_TYPE_SPECIFIER_SIGNED |
					AST_TYPE_SPECIFIER_UNSIGNED);

		switch (type_specifier->specifier) {
			case AST_TYPE_SPECIFIER_VOID:
				if (constraints)
					goto multiset_error;

				// void
				break;

			case AST_TYPE_SPECIFIER_CHAR:
				if (constraints)
					goto multiset_error;

				// char
				break;

			case AST_TYPE_SPECIFIER_SHORT:
				// short int
				if (constraints & ~AST_TYPE_SPECIFIER_INT)
					goto multiset_error;

				// short
				break;

			case AST_TYPE_SPECIFIER_INT:
				if (constraints)
					goto multiset_error;

				// int
				break;

			case AST_TYPE_SPECIFIER_LONG:
				if (
					constraints &
					~(AST_TYPE_SPECIFIER_LONG_INT |
						AST_TYPE_SPECIFIER_DOUBLE__COMPLEX))
					goto multiset_error;

				// long (long (int) | double (_Complex))
				break;

			case AST_TYPE_SPECIFIER_FLOAT:
				if (constraints & ~AST_TYPE_SPECIFIER__COMPLEX)
					goto multiset_error;

				// float (_Complex)
				break;

			case AST_TYPE_SPECIFIER_DOUBLE:
				if (constraints & ~AST_TYPE_SPECIFIER__COMPLEX)
					goto multiset_error;

				// double (_Complex)
				break;

			case AST_TYPE_SPECIFIER_SIGNED:
			case AST_TYPE_SPECIFIER_UNSIGNED:
				if (
					constraints &
					~(AST_TYPE_SPECIFIER_CHAR |
						AST_TYPE_SPECIFIER_LONG_LONG_INT |
						AST_TYPE_SPECIFIER_SHORT_INT))
					goto multiset_error;

				// [signed | unsigned] (short | long | long long) (int)
				break;

			case AST_TYPE_SPECIFIER__BOOL:
				if (constraints)
					goto multiset_error;

				// _Bool
				break;

			case AST_TYPE_SPECIFIER__COMPLEX:
				if (constraints)
					goto multiset_error;

				// _Complex
				break;

			default:
				*error = "unkown type";
				return NULL;
		}

		if (vector_append(&list->type_specifier_vector, &specifier))
			goto vector_error;

		list->type_specifier |= type_specifier->specifier;
	}

	if (type_qualifier) {
		if (list->type_qualifier & type_qualifier->qualifier)
			goto qualifier_error;

		if (vector_append(&list->type_qualifier_vector, &specifier))
			goto vector_error;

		list->type_qualifier |= type_qualifier->qualifier;
	}

	if (function_specifier) {
		if (vector_append(
			&list->function_specifier_vector,
			&specifier)) goto vector_error;

		list->function_specifier |= function_specifier->specifier;
	}

	if (alignment_specifier)
		if (vector_append(
			&list->alignment_specifier_vector,
			&specifier)) goto vector_error;

	list->location.start = location->start;

	return type;

vector_error:
	// at most one node will be appended,
	// so it's okay to just return NULL here
	return NULL;

multiple_storage_class_error:
	*error = "multiple storage class specifiers";

	return NULL;

duplicate_storage_class_error:
	*error = "duplicate storage class";

	return NULL;

thread_local_error:
	*error = "`_Thread_local` may only appear with `static` or `extern`";

	return NULL;

qualifier_error:
	*error = "duplicate qualifier";

	return NULL;

multiset_error:
	*error = "invalid type multiset";

	return NULL;
}

ast_t *ast_type_init(
	ast_t      *specifier,
	location_t *location)
{
	AST_INIT(ast_type_t);

	memset(node, 0, sizeof(*node));

	if (vector_init(
		&node->storage_class_specifier_vector,
		sizeof(specifier),
		0)) goto error;
	if (vector_init(
		&node->type_specifier_vector,
		sizeof(specifier),
		0)) goto error;
	if (vector_init(
		&node->type_qualifier_vector,
		sizeof(specifier),
		0)) goto error;
	if (vector_init(
		&node->function_specifier_vector,
		sizeof(specifier),
		0)) goto error;
	if (vector_init(
		&node->alignment_specifier_vector,
		sizeof(specifier),
		0)) goto error;

	vector_t *vector;
	switch (*specifier) {
		case AST_STORAGE_CLASS_SPECIFIER:
			vector = &node->storage_class_specifier_vector;

			ast_storage_class_specifier_t
				*storage_class_specifier = OFFSETOF_AST_NODE(
					specifier,
					ast_storage_class_specifier_t);

			node->storage_class_specifier =
				storage_class_specifier->specifier;

			break;

		case AST_TYPE_SPECIFIER:
			vector = &node->type_specifier_vector;

			ast_type_specifier_t *type_specifier =
				OFFSETOF_AST_NODE(
					specifier,
					ast_type_specifier_t);

			node->type_specifier = type_specifier->specifier;

			break;

		case AST_TYPE_QUALIFIER:
			vector = &node->type_qualifier_vector;

			ast_type_qualifier_t *type_qualifier =
				OFFSETOF_AST_NODE(
					specifier,
					ast_type_qualifier_t);

			node->type_qualifier = type_qualifier->qualifier;

			break;

		case AST_FUNCTION_SPECIFIER:
			vector = &node->function_specifier_vector;

			ast_function_specifier_t *function_specifier =
				OFFSETOF_AST_NODE(
					specifier,
					ast_function_specifier_t);

			node->function_specifier =
				function_specifier->specifier;

			break;

		case AST_ALIGNAS:
			vector = &node->alignment_specifier_vector;
			break;

		default:
			goto error;
	}

	if (vector_append(vector, &specifier)) goto error;

	node->location = *location;

	AST_RETURN(AST_TYPE);

error:
	vector_free(&node->storage_class_specifier_vector);
	vector_free(&node->type_specifier_vector);
	vector_free(&node->type_qualifier_vector);
	vector_free(&node->function_specifier_vector);
	vector_free(&node->alignment_specifier_vector);

	return NULL;
}

void ast_type_free(ast_t *ast)
{
	AST_FREE(ast_type_t);

	ast_t **storage_class_specifier =
		node->storage_class_specifier_vector.buf;
	ast_t **type_specifier      = node->type_specifier_vector.buf;
	ast_t **type_qualifier      = node->type_qualifier_vector.buf;
	ast_t **function_specifier  = node->function_specifier_vector.buf;
	ast_t **alignment_specifier = node->alignment_specifier_vector.buf;

	for (size_t i = 0; i < node->storage_class_specifier_vector.use; i++)
		AST_NODE_FREE(storage_class_specifier[i]);

	for (size_t i = 0; i < node->type_qualifier_vector.use; i++)
		AST_NODE_FREE(type_qualifier[i]);

	for (size_t i = 0; i < node->type_specifier_vector.use; i++)
		AST_NODE_FREE(type_specifier[i]);

	for (size_t i = 0; i < node->function_specifier_vector.use; i++)
		AST_NODE_FREE(function_specifier[i]);

	for (size_t i = 0; i < node->alignment_specifier_vector.use; i++)
		AST_NODE_FREE(alignment_specifier[i]);

	vector_free(&node->storage_class_specifier_vector);
	vector_free(&node->type_specifier_vector);
	vector_free(&node->type_qualifier_vector);
	vector_free(&node->function_specifier_vector);
	vector_free(&node->alignment_specifier_vector);

	free(node);
}

void fprint_ast_type(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_t);

	if (node->storage_class_specifier_vector.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_STORAGE_CLASS_SPECIFIER],
			node->storage_class_specifier_vector);

	if (node->type_specifier_vector.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_TYPE_SPECIFIER],
			node->type_specifier_vector);

	if (node->type_qualifier_vector.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_TYPE_QUALIFIER],
			node->type_qualifier_vector);

	if (node->function_specifier_vector.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_FUNCTION_SPECIFIER],
			node->function_specifier_vector);

	if (node->alignment_specifier_vector.use)
		FPRINT_AST_REVERSE_LIST(
			ast_node_str[AST_ALIGNAS],
			node->alignment_specifier_vector);

	FPRINT_AST_NODE_FINISH;
}
