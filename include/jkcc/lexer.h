// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.h -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LEXER_H
#define JKCC_LEXER_H


#include <uchar.h>
#include <wchar.h>


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
} integer_constant_t;

typedef struct floating_constant_s {
	enum floating_constant_e type;
	union {
		float       FLOAT;
		double      DOUBLE;
		long double LONG_DOUBLE;
	};
} floating_constant_t;

typedef struct character_constant_s {
	enum character_constant_e type;
	union {
		unsigned char UNSIGNED_CHAR;
		wchar_t       WCHAR_T;
		char16_t      CHAR16_T;
		char32_t      CHAR32_T;
	};
} character_constant_t;


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
int lexer_unsigned_integer_constant(
	const char              *start,
	const char              *end,
	integer_constant_t      *val,
	int                      base,
	enum integer_constant_e  type);


#endif  /* JKCC_LEXER_H */
