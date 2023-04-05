// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.h -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_PRIVATE_AST_H
#define JKCC_PRIVATE_AST_H


#include <jkcc/ast.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jkcc/lexer.h>


#define INDENT(stream, level) if (level) {              \
	char buf[level * 2];                            \
	memset(buf, ' ', sizeof(buf));                  \
	fwrite(buf, sizeof(*buf), sizeof(buf), stream); \
}

#define AST_INIT(type)                      \
	type *node = malloc(sizeof(*node)); \
	if (!node) return NULL;

#define AST_NODE_LOCATION                             \
	node->location.file  = location_start->file;  \
	node->location.start = location_start->start; \
                                                      \
	node->location.end = (location_end)           \
		? location_end->end                   \
		: location_start->end;                \

#define AST_RETURN(val)    \
	node->ast = val;   \
	return &node->ast;

#define AST_FREE(type)                             \
	if (!ast) return;                          \
	type *node = OFFSETOF_AST_NODE(ast, type);

#define FPRINT_AST_BEGIN                            \
	if (!(flags & AST_PRINT_NO_INDENT_INITIAL)) \
		INDENT(stream, level);              \
                                                    \
	fprintf(stream, "{\n");                     \
                                                    \
	++level;

#define FPRINT_AST_NODE_BEGIN(type)                 \
	type *node = OFFSETOF_AST_NODE(ast, type);  \
                                                    \
	FPRINT_AST_BEGIN;

#define FPRINT_AST_FINISH \
	--level;                                      \
                                                      \
	INDENT(stream, level);                        \
	fprintf(stream, "}");                         \
                                                      \
	if (!(flags & AST_PRINT_NO_TRAILING_NEWLINE)) \
		fprintf(stream, "\n");

#define FPRINT_AST_MEMBER(type, member) if (member) { \
	INDENT(stream, level);                        \
	fprintf(stream, "\"%s\" : ", type);           \
						      \
	FPRINT_AST_NODE(                              \
		stream,                               \
		member,                               \
		level,                                \
		AST_PRINT_NO_INDENT_INITIAL |         \
		AST_PRINT_NO_TRAILING_NEWLINE);       \
	fprintf(stream, ",\n");                       \
}

#define FPRINT_AST_FIELD(name, value) {                     \
	INDENT(stream, level);                              \
	fprintf(stream, "\"%s\" : \"%s\",\n", name, value); \
}

#define FPRINT_AST_LIST(name, member) {                 \
	INDENT(stream, level);                          \
	fprintf(stream, "\"%s\" : [\n", name);          \
                                                        \
	++level;                                        \
                                                        \
	ast_t **tmp = member.buf;                       \
                                                        \
	size_t pos;                                     \
	for (pos = 0; pos < member.use - 1; pos++) {    \
		FPRINT_AST_NODE(                        \
			stream,                         \
			tmp[pos],                       \
			level,                          \
			AST_PRINT_NO_TRAILING_NEWLINE); \
                                                        \
		fprintf(stream, ",\n");                 \
	}                                               \
                                                        \
	FPRINT_AST_NODE(stream, tmp[pos], level, 0);    \
                                                        \
	--level;                                        \
                                                        \
	INDENT(stream, level);                          \
	fprintf(stream, "],\n");                        \
}

#define FPRINT_AST_REVERSE_LIST(name, member) {         \
	INDENT(stream, level);                          \
	fprintf(stream, "\"%s\" : [\n", name);          \
                                                        \
	++level;                                        \
                                                        \
	ast_t **tmp = member.buf;                       \
                                                        \
	size_t pos;                                     \
	for (pos = member.use - 1; pos > 0; pos--) {    \
		FPRINT_AST_NODE(                        \
			stream,                         \
			tmp[pos],                       \
			level,                          \
			AST_PRINT_NO_TRAILING_NEWLINE); \
                                                        \
		fprintf(stream, ",\n");                 \
	}                                               \
                                                        \
	FPRINT_AST_NODE(stream, tmp[pos], level, 0);    \
                                                        \
	--level;                                        \
                                                        \
	INDENT(stream, level);                          \
	fprintf(stream, "],\n");                        \
}

#define FPRINT_AST_NODE_FINISH                  \
	INDENT(stream, level);                  \
	fprintf(stream, "\"location\" : ");     \
	fprint_location(                        \
		stream,                         \
		&node->location,                \
		level,                          \
		AST_PRINT_NO_INDENT_INITIAL |   \
		AST_PRINT_NO_TRAILING_NEWLINE); \
                                                \
	fprintf(stream, ",\n");                 \
	INDENT(stream, level);                  \
	fprintf(                                \
		stream,                         \
		"\"ast-type\" : \"%s\"\n",      \
		AST_NODE_STR(&node->ast));      \
                                                \
	FPRINT_AST_FINISH;


#endif  /* JKCC_PRIVATE_AST_H */
