// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * static_declaration.c -- x86 static declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/x86/static_declaration.h>
#include <jkcc/target/x86/util.h>
#include <jkcc/target/util.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ast.h>


int target_x86_static_declaration(
	FILE                    *stream,
	ir_static_declaration_t *static_declaration)
{
	if (*static_declaration->declaration == AST_STRING_LITERAL) {
		fprintf(
			stream,
			"\t.type\t.L%lu,@object\n",
			static_declaration->bb);
		fprintf(stream, "\t.section\t.rodata\n");
		fprintf(stream, ".L%lu:\n", static_declaration->bb);
		size_t size = target_util_string_literal(
			stream,
			static_declaration->declaration);
		if (!size) return -1;
		fprintf(
			stream,
			"\t.size\t.L%lu, %lu\n",
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

	fprintf(stream, "\t.type\t%s,@object\n", identifier->head);
	fprintf(stream, "\t.bss\n");
	fprintf(stream, "\t.local\t%s\n", identifier->head);
	fprintf(stream, "\t.p2align\t2, 0x0\n");
	fprintf(stream, "%s:\n", identifier->head);
	fprintf(stream, "\t.size\t%s, %lu\n", identifier->head, size);

	return 0;
}
