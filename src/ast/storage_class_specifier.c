// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * storage_class_specifier.c -- storage_class_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast/ast.h>
#include <jkcc/ast/storage_class_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_storage_class_specifier_init(
	uint_fast8_t  specifier,
	location_t   *location)
{
	AST_INIT(ast_storage_class_specifier_t);

	node->specifier =  specifier;
	node->location  = *location;

	AST_RETURN(AST_STORAGE_CLASS_SPECIFIER);
}

void ast_storage_class_specifier_free(ast_t *ast)
{
	AST_FREE(ast_storage_class_specifier_t);

	free(node);
}

void fprint_ast_storage_class_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_storage_class_specifier_t);

	const char *specifier;
	switch (node->specifier) {
		case AST_STORAGE_CLASS_SPECIFIER_TYPEDEF:
			specifier = "typedef";
			break;

		case AST_STORAGE_CLASS_SPECIFIER_EXTERN:
			specifier = "extern";
			break;

		case AST_STORAGE_CLASS_SPECIFIER_STATIC:
			specifier = "static";
			break;

		case AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL:
			specifier = "_Thread_local";
			break;

		case AST_STORAGE_CLASS_SPECIFIER_AUTO:
			specifier = "auto";
			break;

		case AST_STORAGE_CLASS_SPECIFIER_REGISTER:
			specifier = "register";
			break;

		default:
			specifier = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("specifier", specifier);

	FPRINT_AST_NODE_FINISH;
}
