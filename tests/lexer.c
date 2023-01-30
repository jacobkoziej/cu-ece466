// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.c -- lexer unit tests
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>


static void null_test_success(void **state)
{
	(void) state;
}


int main(void)
{
	static const struct CMUnitTest tests[] = {
		cmocka_unit_test(null_test_success),
	};


	return cmocka_run_group_tests(tests, NULL, NULL);
}
