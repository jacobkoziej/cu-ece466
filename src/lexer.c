// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.c -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/lexer.h>

#include <errno.h>
#include <limits.h>
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
