// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * type_specifier.c -- type_specifier ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/type_specifier.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/location.h>


ast_t *ast_type_specifier_init(
	uint_fast16_t  specifier,
	ast_t         *semantic_type,
	location_t    *location)
{
	AST_INIT(ast_type_specifier_t);

	node->specifier     =  specifier;
	node->semantic_type =  semantic_type;
	node->location      = *location;

	AST_RETURN(AST_TYPE_SPECIFIER);
}

void ast_type_specifier_free(ast_t *ast)
{
	AST_FREE(ast_type_specifier_t);

	AST_NODE_FREE(node->semantic_type);

	free(node);
}

void fprint_ast_type_specifier(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_type_specifier_t);

	const char *specifier;

	if (node->semantic_type) {
		specifier = "(semantic)";
	} else {
		switch (node->specifier) {
			case AST_TYPE_SPECIFIER_VOID:
				specifier = "void";
				break;

			case AST_TYPE_SPECIFIER_CHAR:
				specifier = "char";
				break;

			case AST_TYPE_SPECIFIER_SHORT:
				specifier = "short";
				break;

			case AST_TYPE_SPECIFIER_INT:
				specifier = "int";
				break;

			case AST_TYPE_SPECIFIER_LONG:
				specifier = "long";
				break;

			case AST_TYPE_SPECIFIER_FLOAT:
				specifier = "float";
				break;

			case AST_TYPE_SPECIFIER_DOUBLE:
				specifier = "double";
				break;

			case AST_TYPE_SPECIFIER_SIGNED:
				specifier = "signed";
				break;

			case AST_TYPE_SPECIFIER_UNSIGNED:
				specifier = "unsigned";
				break;

			case AST_TYPE_SPECIFIER__BOOL:
				specifier = "_Bool";
				break;

			case AST_TYPE_SPECIFIER__COMPLEX:
				specifier = "_Complex";
				break;

			default:
				specifier = "(unknown)";
				break;
		}
	}

	FPRINT_AST_FIELD("specifier", specifier);

	FPRINT_AST_NODE_FINISH;
}
