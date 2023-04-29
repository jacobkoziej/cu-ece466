// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.h -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LEXER_H
#define JKCC_LEXER_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <uchar.h>
#include <wchar.h>

#include <jkcc/ast/ast.h>
#include <jkcc/constant.h>
#include <jkcc/list.h>
#include <jkcc/location.h>
#include <jkcc/scope.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


typedef struct yyextra_s {
	file_t                    *file;
	vector_t                  *file_allocated;
	int                        integer_constant_base;
	enum character_constant_e  character_constant_type;
	int                        prev_yylineno;
	const char                *error;
	ast_t                     *identifier;
	bool                       prescope_declaration;
	bool                       variadic_parameter;
	uint_fast16_t              identifier_type;
	scope_t                   *symbol_table;
} yyextra_t;


int lexer_character_constant(
	const char                *start,
	const char                *end,
	character_constant_t      *val,
	int                        base,
	enum character_constant_e  type);
int lexer_floating_constant(
	const char               *start,
	const char               *end,
	floating_constant_t      *val,
	enum floating_constant_e  type);
int lexer_signed_integer_constant(
	const char              *start,
	const char              *end,
	integer_constant_t      *val,
	int                      base,
	enum integer_constant_e  type);
int lexer_universal_character_name(
	const char *start,
	const char *end,
	uint32_t   *val);
int lexer_unsigned_integer_constant(
	const char              *start,
	const char              *end,
	integer_constant_t      *val,
	int                      base,
	enum integer_constant_e  type);
void lexer_utf32_to_utf8(
	char     *buf,
	size_t   *bytes,
	uint32_t  ucn);


#endif  /* JKCC_LEXER_H */
