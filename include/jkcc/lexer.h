// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.h -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LEXER_H
#define JKCC_LEXER_H


#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <uchar.h>
#include <wchar.h>

#include <jkcc/list.h>
#include <jkcc/string.h>
#include <jkcc/vector.h>


enum integer_constant_e {
	INT,
	UNSIGNED_INT,
	LONG_INT,
	UNSIGNED_LONG_INT,
	LONG_LONG_INT,
	UNSIGNED_LONG_LONG_INT,
};

enum floating_constant_e {
	FLOAT,
	DOUBLE,
	LONG_DOUBLE,
};

enum character_constant_e {
	UNSIGNED_CHAR,
	WCHAR_T,
	CHAR16_T,
	CHAR32_T,
};

enum string_literal_e {
	STRING_CHAR,
	STRING_UTF_8,
	STRING_CHAR16_T,
	STRING_CHAR32_T,
	STRING_WCHAR_T,
};


typedef struct file_s {
	char   *path;
	list_t  list;
	size_t  refs;
} file_t;

typedef struct location_s {
	file_t *file;
	struct {
		off_t offset;
		int   line;
		int   column;
	} start;
	struct {
		off_t offset;
		int   line;
		int   column;
	} end;
} location_t;

typedef struct identifier_s {
	string_t IDENTIFIER;
	string_t text;
} identifier_t;

typedef struct integer_constant_s {
	enum integer_constant_e type;
	union {
		int                    INT;
		unsigned int           UNSIGNED_INT;
		long int               LONG_INT;
		unsigned long int      UNSIGNED_LONG_INT;
		long long int          LONG_LONG_INT;
		unsigned long long int UNSIGNED_LONG_LONG_INT;
	};
	string_t text;
} integer_constant_t;

typedef struct floating_constant_s {
	enum floating_constant_e type;
	union {
		float       FLOAT;
		double      DOUBLE;
		long double LONG_DOUBLE;
	};
	string_t text;
} floating_constant_t;

typedef struct character_constant_s {
	enum character_constant_e type;
	union {
		unsigned char UNSIGNED_CHAR;
		wchar_t       WCHAR_T;
		char16_t      CHAR16_T;
		char32_t      CHAR32_T;
	};
	string_t text;
} character_constant_t;

typedef struct string_literal_s {
	enum string_literal_e encoding;
	string_t              string;
	string_t              text;
} string_literal_t;

typedef struct yyextra_s {
	file_t                    *file;
	vector_t                  *file_allocated;
	int                        integer_constant_base;
	enum character_constant_e  character_constant_type;
	int                        prev_yylineno;
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
