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
#include <jkcc/vector.h>

#include "y.tab.h"
#include "lex.yy.h"


translation_unit_t *parse(parser_t *parser)
{
	translation_unit_t *translation_unit;
	file_t             *file;
	yyscan_t            yyscanner = NULL;
	FILE               *stream    = NULL;

	translation_unit = calloc(1, sizeof(*translation_unit));
	if (!translation_unit) return NULL;

	if (vector_init(&translation_unit->file, sizeof(file), 0)) goto error;

	file = calloc(1, sizeof(*file));
	if (!file) goto error;

	file->path = strdup((parser->path) ? parser->path : "/dev/stdin");
	if (!file->path) goto error;

	if (vector_append(&translation_unit->file, &file)) goto error;

	// avoid a double free in error recovery
	file = NULL;

	stream = (parser->path) ? fopen(parser->path, "r") : stdin;
	if (!stream) goto error;

	yyextra_t yyextra_data = {
		.file           = file,
		.file_allocated = &translation_unit->file,
	};

	if (yylex_init(&yyscanner)) goto error;
	if (yylex_init_extra(&yyextra_data, yyscanner)) goto error;

	yyrestart(stream, yyscanner);

	if (yyparse(yyscanner, parser, translation_unit)) goto error;

	yylex_destroy(yyscanner);

	if (stream != stdin) fclose(stream);

	return translation_unit;

error:
	if (yyscanner) yylex_destroy(yyscanner);

	if (stream && (stream != stdin)) fclose(stream);

	if (file) {
		if (file->path) free(file->path);
		free(file);
	}

	if (translation_unit->file.buf) {
		file_t **file = translation_unit->file.buf;

		for (size_t i = 0; i < translation_unit->file.use; i++) {
			free(file[i]->path);
			free(file[i]);
		}

		vector_free(&translation_unit->file);
	}

	free(translation_unit);

	return NULL;
}

void translation_unit_free(translation_unit_t *translation_unit)
{
	AST_NODE_FREE(translation_unit->ast);

	file_t **file = translation_unit->file.buf;

	for (size_t i = 0; i < translation_unit->file.use; i++) {
		free(file[i]->path);
		free(file[i]);
	}

	free(translation_unit);
}
