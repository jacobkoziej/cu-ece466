// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_declaration.c -- x86 static declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/x86/static_declaration.h>
#include <jkcc/target/x86/util.h>
#include <jkcc/target/util.h>
#include <jkcc/private/target.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ast.h>


int target_x86_static_declaration(
	FILE                    *stream,
	ir_static_declaration_t *static_declaration)
{
	if (*static_declaration->declaration == AST_STRING_LITERAL) {
		DIRECTIVE_ARG(".type\t.L%lu,@object", static_declaration->bb);
		DIRECTIVE(".section\t.rodata");
		LOCAL_LABEL(static_declaration->bb);
		size_t size = target_util_string_literal(
			stream,
			static_declaration->declaration);
		if (!size) return -1;
		DIRECTIVE_ARG(
			".size\t.L%lu, %lu",
			static_declaration->bb,
			size);

		return 0;
	}

	ast_t *ast_identifier = ast_declaration_get_identifier(
		static_declaration->declaration);
	ast_t *ast_type = ast_declaration_get_type(
		static_declaration->declaration);

	const string_t *identifier = ast_identifier_get_string(ast_identifier);

	size_t size = target_x86_util_sizeof(ast_type);
	if (!size) return -1;

	DIRECTIVE_ARG(".type\t%s,@object", identifier->head);
	DIRECTIVE(".bss");
	DIRECTIVE_ARG(".local\t%s", identifier->head);
	DIRECTIVE(".p2align\t2, 0x0");
	LABEL(identifier->head);
	DIRECTIVE_ARG(".size\t%s, %lu", identifier->head, size);

	return 0;
}
