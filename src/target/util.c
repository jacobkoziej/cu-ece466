// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * util.c -- target code generation utils
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/util.h>

#include <stddef.h>
#include <stdio.h>

#include <jkcc/ast.h>
#include <jkcc/constant.h>


size_t target_util_string_literal(FILE *stream, ast_t *ast)
{
	if (*ast != AST_STRING_LITERAL) return 0;

	const string_literal_t *string_literal
		= ast_string_literal_get_string_literal(ast);

	fprintf(stream, "\t.asciz\t%s\n", string_literal->text.head);

	return string_literal->string.tail - string_literal->string.head + 1;
}
