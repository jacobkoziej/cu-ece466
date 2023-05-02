// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.c -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/parser.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <jkcc/ast.h>
#include <jkcc/lexer.h>
#include <jkcc/location.h>
#include <jkcc/scope.h>
#include <jkcc/vector.h>

#include "y.tab.h"
#include "lex.yy.h"


ast_t *parse(parser_t *parser)
{
	vector_t *base_type      = NULL;
	file_t   *file           = NULL;
	vector_t *file_allocated = NULL;
	yyscan_t  yyscanner      = NULL;
	FILE     *stream         = NULL;
	vector_t *goto_list      = NULL;
	vector_t *type_stack     = NULL;
	scope_t  *symbol_table   = NULL;

	ast_t *translation_unit = ast_translation_unit_init();
	if (!translation_unit) return NULL;

	file_allocated = ast_translation_unit_get_file(translation_unit);
	base_type      = ast_translation_unit_get_base_type(translation_unit);

	file = calloc(1, sizeof(*file));
	if (!file) goto error;

	file->path = strdup((parser->path) ? parser->path : "/dev/stdin");
	if (!file->path) goto error;

	if (vector_append(file_allocated, &file)) goto error;

	// avoid a double free in error recovery
	file = NULL;

	symbol_table = scope_init();
	if (!symbol_table) goto error;

	symbol_table->context.current.storage_class =
		AST_DECLARATION_IMPLICIT_EXTERN;
	symbol_table->context.base.storage_class =
		AST_DECLARATION_IMPLICIT_EXTERN;

	stream = (parser->path) ? fopen(parser->path, "r") : stdin;
	if (!stream) goto error;

	yyextra_t yyextra_data = {
		.file             = *(file_t**) file_allocated->buf,
		.file_allocated   = file_allocated,
		.base_type        = base_type,
		.symbol_table     = symbol_table,
		.translation_unit = translation_unit,
	};

	goto_list = &yyextra_data.goto_list;
	if (vector_init(goto_list, sizeof(ast_t*), 0)) goto error;

	type_stack = &yyextra_data.type_stack;
	if (vector_init(type_stack, sizeof(ast_t*), 0)) goto error;

	if (yylex_init_extra(&yyextra_data, &yyscanner)) goto error;

	yyrestart(stream, yyscanner);

	if (yyparse(yyscanner, parser, &yyextra_data)) goto error;

	yylex_destroy(yyscanner);

	vector_free(type_stack);
	vector_free(goto_list);

	scope_free(symbol_table);

	if (stream != stdin) fclose(stream);

	return translation_unit;

error:
	if (yyscanner) yylex_destroy(yyscanner);

	vector_free(type_stack);
	vector_free(goto_list);

	if (stream && (stream != stdin)) fclose(stream);

	scope_free(symbol_table);

	if (file) {
		if (file->path) free(file->path);
		free(file);
	}

	AST_NODE_FREE(translation_unit);

	return NULL;
}
