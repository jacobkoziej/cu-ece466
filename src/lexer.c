// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.c -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/lexer.h>

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>


int lexer_character_constant(
	const char                *start,
	const char                *end,
	character_constant_t      *val,
	int                        base,
	enum character_constant_e  type)
{
	(void) end;

	errno = 0;
	unsigned long long int tmp = strtoull(start, NULL, base);
	if (errno) goto error;

	val->type = type;
	switch (type) {
		case UNSIGNED_CHAR:
			val->UNSIGNED_CHAR = tmp;
			break;

		case WCHAR_T:
			val->WCHAR_T = tmp;
			break;

		case CHAR16_T:
			val->CHAR16_T = tmp;
			break;

		case CHAR32_T:
			val->CHAR32_T = tmp;
			break;

		default:
			goto error;
	}

	return 0;

error:
	return -1;
}

int lexer_floating_constant(
	const char               *start,
	const char               *end,
	floating_constant_t      *val,
	enum floating_constant_e  type)
{
	(void) end;

	errno = 0;

	switch (type) {
		case FLOAT: {
			float tmp = strtof(start, NULL);
			if (errno) goto error;

			val->type = FLOAT;
			val->FLOAT = tmp;
			break;
		}

		case DOUBLE: {
			double tmp = strtod(start, NULL);
			if (errno) goto error;

			val->type = DOUBLE;
			val->DOUBLE = tmp;
			break;
		}

		case LONG_DOUBLE: {
			double tmp = strtold(start, NULL);
			if (errno) goto error;

			val->type = LONG_DOUBLE;
			val->LONG_DOUBLE = tmp;
			break;
		}

		default:
			goto error;
	}

	return 0;

error:
	return -1;
}

int lexer_signed_integer_constant(
	const char              *start,
	const char              *end,
	integer_constant_t      *val,
	int                      base,
	enum integer_constant_e  type)
{
	(void) end;

	errno = 0;
	long long tmp = strtoll(start, NULL, base);
	if (errno) goto error;

	if ((type <= INT) && (tmp <= INT_MAX)) {
		val->type = INT;
		val->INT = tmp;
		goto done;
	}

	if ((type <= LONG_INT) && (tmp <= LONG_MAX)) {
		val->type = LONG_INT;
		val->LONG_INT = tmp;
		goto done;
	}

	if ((type <= LONG_LONG_INT) && (tmp <= LLONG_MAX)) {
		val->type = LONG_LONG_INT;
		val->LONG_LONG_INT = tmp;
		goto done;
	}

done:
	return 0;

error:
	return -1;
}

int lexer_universal_character_name(
	const char *start,
	const char *end,
	uint32_t   *val)
{
	(void) end;

	errno = 0;
	unsigned long long int tmp = strtoull(start, NULL, 16);
	if (errno) goto error;

	if (tmp < 0x00a0)
		if ((tmp != 0x0024) && (tmp != 0x0040) && (tmp != 0x0060))
			goto error;

	if ((tmp >= 0xd800) && (tmp <= 0xdfff)) goto error;

	*val = tmp;

	return 0;

error:
	return -1;
}

int lexer_unsigned_integer_constant(
	const char             *start,
	const char             *end,
	integer_constant_t     *val,
	int                     base,
	enum integer_constant_e type)
{
	(void) end;

	errno = 0;
	unsigned long long int tmp = strtoull(start, NULL, base);
	if (errno) goto error;

	if ((type <= UNSIGNED_INT) && (tmp <= UINT_MAX)) {
		val->type = UNSIGNED_INT;
		val->UNSIGNED_INT = tmp;
		goto done;
	}

	if ((type <= UNSIGNED_LONG_INT) && (tmp <= ULONG_MAX)) {
		val->type = UNSIGNED_LONG_INT;
		val->UNSIGNED_LONG_INT = tmp;
		goto done;
	}

	if ((type <= UNSIGNED_LONG_LONG_INT) && (tmp <= ULLONG_MAX)) {
		val->type = UNSIGNED_LONG_LONG_INT;
		val->UNSIGNED_LONG_LONG_INT = tmp;
		goto done;
	}

done:
	return 0;

error:
	return -1;
}

void lexer_utf32_to_utf8(
	char     *buf,
	size_t   *bytes,
	uint32_t  ucn)
{
	// inspired by: https://github.com/llvm/llvm-project/blob/2ad0cc9186d4952b3009e1b8c3c606f0b13ea38a/clang/lib/Lex/LiteralSupport.cpp#L739

	size_t bytes_to_write;
	if      (ucn < 0x80)    bytes_to_write = 1;
	else if (ucn < 0x800)   bytes_to_write = 2;
	else if (ucn < 0x10000) bytes_to_write = 3;
	else                    bytes_to_write = 4;

	static const unsigned byte_mask = 0xbf;
	static const unsigned byte_mark = 0x80;

	static const uint8_t first_byte_mark[5] = {
		0x00,
		0x00,
		0xc0,
		0xe0,
		0xf0,
	};

	buf += bytes_to_write;
	for (size_t i = 1; i < bytes_to_write; i++) {
		*--buf   = ((ucn | byte_mark) & byte_mask);
		ucn    >>= 6;
	}
	*--buf = (ucn | first_byte_mark[bytes_to_write]);

	*bytes = bytes_to_write;
}
