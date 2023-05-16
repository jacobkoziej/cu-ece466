// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * extern_declaration.c -- x86 extern declaration target code generation
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/x86/extern_declaration.h>
#include <jkcc/target/x86/util.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ast.h>


int target_x86_extern_declaration(FILE *stream, ast_t *ast)
{
	ast_t *ast_identifier = ast_declaration_get_identifier(ast);
	ast_t *ast_type       = ast_declaration_get_type(ast);

	const string_t *identifier = ast_identifier_get_string(ast_identifier);

	size_t size = target_x86_util_sizeof(ast_type);
	if (!size) return -1;

	fprintf(stream, "\t.type\t%s,@object\n", identifier->head);
	fprintf(stream, "\t.bss\n");
	fprintf(stream, "\t.globl\t%s\n", identifier->head);
	fprintf(stream, "\t.p2align\t2, 0x0\n");
	fprintf(stream, "%s:\n", identifier->head);
	fprintf(stream, "\t.size\t%s, %lu\n", identifier->head, size);

	return 0;
}
