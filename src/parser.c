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
#include <jkcc/symbol.h>
#include <jkcc/vector.h>

#include "y.tab.h"
#include "lex.yy.h"


translation_unit_t *parse(parser_t *parser)
{
	translation_unit_t *translation_unit;
	file_t             *file;
	symbol_table_t     *identifier_symbol_table;
	yyscan_t            yyscanner = NULL;
	FILE               *stream    = NULL;

	translation_unit = calloc(1, sizeof(*translation_unit));
	if (!translation_unit) return NULL;

	if (vector_init(&translation_unit->file, sizeof(file), 0)) goto error;
	if (vector_init(
		&translation_unit->symbol_table.identifier,
		sizeof(translation_unit->ast),
		0)) goto error;

	file = calloc(1, sizeof(*file));
	if (!file) goto error;

	file->path = strdup((parser->path) ? parser->path : "/dev/stdin");
	if (!file->path) goto error;

	if (vector_append(&translation_unit->file, &file)) goto error;

	// avoid a double free in error recovery
	file = NULL;

	identifier_symbol_table = symbol_init();
	if (!identifier_symbol_table) goto error;

	if (vector_append(
		&translation_unit->symbol_table.identifier,
		&identifier_symbol_table)) goto error;

	// avoid a double free in error recovery
	identifier_symbol_table = NULL;

	stream = (parser->path) ? fopen(parser->path, "r") : stdin;
	if (!stream) goto error;

	yyextra_t yyextra_data = {
		.file           = *(file_t**) translation_unit->file.buf,
		.file_allocated = &translation_unit->file,
		.symbol_table   = {
			.identifier = *(symbol_table_t**)
				translation_unit->symbol_table.identifier.buf,
		},
	};

	parser->yyextra_data = &yyextra_data;

	if (yylex_init_extra(&yyextra_data, &yyscanner)) goto error;

	yyrestart(stream, yyscanner);

	if (yyparse(yyscanner, parser, translation_unit)) goto error;

	yylex_destroy(yyscanner);

	if (stream != stdin) fclose(stream);

	return translation_unit;

error:
	if (yyscanner) yylex_destroy(yyscanner);

	if (stream && (stream != stdin)) fclose(stream);

	symbol_free(identifier_symbol_table);

	if (file) {
		if (file->path) free(file->path);
		free(file);
	}

	free(translation_unit);

	return NULL;
}

int parse_insert_identifier(
	parser_t *parser,
	ast_t    *identifier,
	ast_t    *type)
{
	symbol_table_t *symbol = parser->yyextra_data->symbol_table.identifier;

	ast_identifier_set_type(identifier, type);
	const string_t *key = ast_identifier_get_string(identifier);

	return symbol_insert(symbol, key->head, key->tail - key->head, type);
}

void translation_unit_free(translation_unit_t *translation_unit)
{
	if (!translation_unit) return;

	AST_NODE_FREE(translation_unit->ast);

	file_t **file = translation_unit->file.buf;

	for (size_t i = 0; i < translation_unit->file.use; i++) {
		free(file[i]->path);
		free(file[i]);
	}

	symbol_table_t **symbol;

	symbol = translation_unit->symbol_table.identifier.buf;
	for (
		size_t i = 0;
		i < translation_unit->symbol_table.identifier.use;
		i++)
		symbol_free(symbol[i]);

	vector_free(&translation_unit->file);
	vector_free(&translation_unit->symbol_table.identifier);

	free(translation_unit);
}
