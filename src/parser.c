// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.c -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/parser.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <jkcc/ast/ast.h>
#include <jkcc/lexer.h>
#include <jkcc/location.h>
#include <jkcc/scope.h>
#include <jkcc/vector.h>

#include "y.tab.h"
#include "lex.yy.h"


translation_unit_t *parse(parser_t *parser)
{
	translation_unit_t *translation_unit;
	file_t             *file;
	yyscan_t            yyscanner  = NULL;
	FILE               *stream     = NULL;
	vector_t           *type_stack = NULL;

	translation_unit = calloc(1, sizeof(*translation_unit));
	if (!translation_unit) return NULL;

	if (vector_init(&translation_unit->file, sizeof(file), 0)) goto error;
	if (vector_init(&translation_unit->base_type, sizeof(ast_t*), 0))
		goto error;

	file = calloc(1, sizeof(*file));
	if (!file) goto error;

	file->path = strdup((parser->path) ? parser->path : "/dev/stdin");
	if (!file->path) goto error;

	if (vector_append(&translation_unit->file, &file)) goto error;

	// avoid a double free in error recovery
	file = NULL;

	translation_unit->symbol_table = scope_init();
	if (!translation_unit->symbol_table) goto error;

	translation_unit->symbol_table->context.current.storage_class =
		AST_DECLARATION_IMPLICIT_EXTERN;
	translation_unit->symbol_table->context.base.storage_class =
		AST_DECLARATION_IMPLICIT_EXTERN;

	stream = (parser->path) ? fopen(parser->path, "r") : stdin;
	if (!stream) goto error;

	yyextra_t yyextra_data = {
		.file           = *(file_t**) translation_unit->file.buf,
		.file_allocated = &translation_unit->file,
		.symbol_table   = translation_unit->symbol_table,
	};

	type_stack = &yyextra_data.type_stack;
	if (vector_init(type_stack, sizeof(ast_t*), 0)) goto error;

	parser->yyextra_data = &yyextra_data;

	if (yylex_init_extra(&yyextra_data, &yyscanner)) goto error;

	yyrestart(stream, yyscanner);

	if (yyparse(yyscanner, parser, translation_unit)) goto error;

	yylex_destroy(yyscanner);

	vector_free(type_stack);

	if (stream != stdin) fclose(stream);

	return translation_unit;

error:
	if (yyscanner) yylex_destroy(yyscanner);

	vector_free(type_stack);

	if (stream && (stream != stdin)) fclose(stream);

	scope_free(translation_unit->symbol_table);

	if (file) {
		if (file->path) free(file->path);
		free(file);
	}

	free(translation_unit);

	return NULL;
}

void translation_unit_free(translation_unit_t *translation_unit)
{
	if (!translation_unit) return;

	if (translation_unit->ast && *translation_unit->ast != AST_TYPE)
		AST_NODE_FREE(translation_unit->ast);

	scope_free(translation_unit->symbol_table);

	file_t **file = translation_unit->file.buf;
	for (size_t i = 0; i < translation_unit->file.use; i++) {
		free(file[i]->path);
		free(file[i]);
	}

	ast_t **type = translation_unit->base_type.buf;
	for (size_t i = 0; i < translation_unit->base_type.use; i++)
		AST_NODE_FREE(type[translation_unit->base_type.use - i - 1]);

	vector_free(&translation_unit->file);
	vector_free(&translation_unit->base_type);

	free(translation_unit);
}
