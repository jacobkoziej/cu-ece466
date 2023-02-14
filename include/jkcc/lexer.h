// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.h -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_LEXER_H
#define JKCC_LEXER_H


enum integer_constant_e {
	INT,
	UNSIGNED_INT,
	LONG_INT,
	UNSIGNED_LONG_INT,
	LONG_LONG_INT,
	UNSIGNED_LONG_LONG_INT,
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


int lexer_signed_integer_constant(
	const char         *start,
	const char         *end,
	integer_constant_t *val,
	int                 base,
	long long int       min_max_size);
int lexer_unsigned_integer_constant(
	const char             *start,
	const char             *end,
	integer_constant_t     *val,
	int                     base,
	unsigned long long int  min_max_size);


#endif  /* JKCC_LEXER_H */
