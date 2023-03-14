// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.c -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/parser.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <jkcc/lexer.h>
#include <jkcc/vector.h>

#include "y.tab.h"
#include "lex.yy.h"


parse_t *parse(const char *path)
{
	parse_t  *out;
	file_t   *file;
	yyscan_t  yyscanner = NULL;
	FILE     *stream    = NULL;

	out = calloc(1, sizeof(*out));
	if (!out) return NULL;

	if (vector_init(&out->file, sizeof(*file), 0)) goto error;

	file = calloc(1, sizeof(*file));
	if (!file) goto error;

	file->path = strdup((path) ? path : "/dev/stdin");
	if (!file->path) goto error;

	if (vector_append(&out->file, file)) goto error;

	// avoid a double free in error recovery
	file = NULL;

	stream = (path) ? fopen(path, "r") : stdin;
	if (!stream) goto error;

	yyextra_t yyextra_data = {
		.file           = file,
		.file_allocated = &out->file,
	};

	if (yylex_init(&yyscanner)) goto error;
	if (yylex_init_extra(&yyextra_data, yyscanner)) goto error;

	yyrestart(stream, yyscanner);

	if (yyparse(yyscanner, out)) goto error;

	yylex_destroy(yyscanner);

	if (stream != stdin) fclose(stream);

	return out;

error:
	if (yyscanner) yylex_destroy(yyscanner);

	if (stream && (stream != stdin)) fclose(stream);

	if (file) {
		if (file->path) free(file->path);
		free(file);
	}

	if (out->file.buf) {
		file_t **file = out->file.buf;

		for (size_t i = 0; i < out->file.use; i++) {
			free(file[i]->path);
			free(file[i]);
		}

		vector_free(&out->file);
	}

	free(out);

	return NULL;
}
