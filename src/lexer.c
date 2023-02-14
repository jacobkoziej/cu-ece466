// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.c -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/lexer.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>


int lexer_unsigned_integer_constant(
	const char             *start,
	const char             *end,
	integer_constant_t     *val,
	int                     base,
	unsigned long long int  min_max_size)
{
	(void) end;

	errno = 0;
	unsigned long long int tmp = strtoull(start, NULL, base);
	if (errno) goto error;

	if ((min_max_size <= UINT_MAX) && (tmp <= UINT_MAX)) {
		val->type = UNSIGNED_INT;
		val->UNSIGNED_INT = tmp;
		goto done;
	}

	if ((min_max_size <= ULONG_MAX) && (tmp <= ULONG_MAX)) {
		val->type = UNSIGNED_LONG_INT;
		val->UNSIGNED_LONG_INT = tmp;
		goto done;
	}

	if ((min_max_size <= ULLONG_MAX) && (tmp <= ULLONG_MAX)) {
		val->type = UNSIGNED_LONG_LONG_INT;
		val->UNSIGNED_LONG_LONG_INT = tmp;
		goto done;
	}

done:
	return 0;

error:
	return -1;
}
