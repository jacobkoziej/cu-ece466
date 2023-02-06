// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * lexer.c -- lexer unit tests
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <cmocka.h>

#include "lex.yy.h"
#include "parser.tab.h"


static char **yyin_next;


static int setup(void **state)
{
	(void) state;

	yyrestart(fopen(*yyin_next++, "r"));

	return 0;
}

static int teardown(void **state)
{
	(void) state;

	fclose(yyin);

	return 0;
}


static void test_identifiers(void **state)
{
	(void) state;

	assert_int_equal(yylex(),   IDENTIFIER_GENERAL);
	assert_string_equal(yytext, "_start");
	assert_int_equal(yylex(),   IDENTIFIER___FUNC__);
	assert_string_equal(yytext, "__func__");
	assert_int_equal(yylex(),   IDENTIFIER_UNIVERSAL_CHARACTER_NAME);
	assert_string_equal(yytext, "\\u3164");
	assert_int_equal(yylex(),   IDENTIFIER_UNIVERSAL_CHARACTER_NAME);
	assert_string_equal(yytext, "\\U0001f60f");
}

static void test_keywords(void **state)
{
	(void) state;

	assert_int_equal(yylex(),   KEYWORD_AUTO);
	assert_string_equal(yytext, "auto");
	assert_int_equal(yylex(),   KEYWORD_BREAK);
	assert_string_equal(yytext, "break");
	assert_int_equal(yylex(),   KEYWORD_CASE);
	assert_string_equal(yytext, "case");
	assert_int_equal(yylex(),   KEYWORD_CHAR);
	assert_string_equal(yytext, "char");
	assert_int_equal(yylex(),   KEYWORD_CONST);
	assert_string_equal(yytext, "const");
	assert_int_equal(yylex(),   KEYWORD_CONTINUE);
	assert_string_equal(yytext, "continue");
	assert_int_equal(yylex(),   KEYWORD_DEFAULT);
	assert_string_equal(yytext, "default");
	assert_int_equal(yylex(),   KEYWORD_DO);
	assert_string_equal(yytext, "do");
	assert_int_equal(yylex(),   KEYWORD_DOUBLE);
	assert_string_equal(yytext, "double");
	assert_int_equal(yylex(),   KEYWORD_ELSE);
	assert_string_equal(yytext, "else");
	assert_int_equal(yylex(),   KEYWORD_ENUM);
	assert_string_equal(yytext, "enum");
	assert_int_equal(yylex(),   KEYWORD_EXTERN);
	assert_string_equal(yytext, "extern");
	assert_int_equal(yylex(),   KEYWORD_FLOAT);
	assert_string_equal(yytext, "float");
	assert_int_equal(yylex(),   KEYWORD_FOR);
	assert_string_equal(yytext, "for");
	assert_int_equal(yylex(),   KEYWORD_GOTO);
	assert_string_equal(yytext, "goto");
	assert_int_equal(yylex(),   KEYWORD_IF);
	assert_string_equal(yytext, "if");
	assert_int_equal(yylex(),   KEYWORD_INLINE);
	assert_string_equal(yytext, "inline");
	assert_int_equal(yylex(),   KEYWORD_INT);
	assert_string_equal(yytext, "int");
	assert_int_equal(yylex(),   KEYWORD_LONG);
	assert_string_equal(yytext, "long");
	assert_int_equal(yylex(),   KEYWORD_REGISTER);
	assert_string_equal(yytext, "register");
	assert_int_equal(yylex(),   KEYWORD_RESTRICT);
	assert_string_equal(yytext, "restrict");
	assert_int_equal(yylex(),   KEYWORD_RETURN);
	assert_string_equal(yytext, "return");
	assert_int_equal(yylex(),   KEYWORD_SHORT);
	assert_string_equal(yytext, "short");
	assert_int_equal(yylex(),   KEYWORD_SIGNED);
	assert_string_equal(yytext, "signed");
	assert_int_equal(yylex(),   KEYWORD_SIZEOF);
	assert_string_equal(yytext, "sizeof");
	assert_int_equal(yylex(),   KEYWORD_STATIC);
	assert_string_equal(yytext, "static");
	assert_int_equal(yylex(),   KEYWORD_STRUCT);
	assert_string_equal(yytext, "struct");
	assert_int_equal(yylex(),   KEYWORD_SWITCH);
	assert_string_equal(yytext, "switch");
	assert_int_equal(yylex(),   KEYWORD_TYPEDEF);
	assert_string_equal(yytext, "typedef");
	assert_int_equal(yylex(),   KEYWORD_UNION);
	assert_string_equal(yytext, "union");
	assert_int_equal(yylex(),   KEYWORD_UNSIGNED);
	assert_string_equal(yytext, "unsigned");
	assert_int_equal(yylex(),   KEYWORD_VOID);
	assert_string_equal(yytext, "void");
	assert_int_equal(yylex(),   KEYWORD_VOLATILE);
	assert_string_equal(yytext, "volatile");
	assert_int_equal(yylex(),   KEYWORD_WHILE);
	assert_string_equal(yytext, "while");
	assert_int_equal(yylex(),   KEYWORD__ALIGNAS);
	assert_string_equal(yytext, "_Alignas");
	assert_int_equal(yylex(),   KEYWORD__ALIGNOF);
	assert_string_equal(yytext, "_Alignof");
	assert_int_equal(yylex(),   KEYWORD__ATOMIC);
	assert_string_equal(yytext, "_Atomic");
	assert_int_equal(yylex(),   KEYWORD__BOOL);
	assert_string_equal(yytext, "_Bool");
	assert_int_equal(yylex(),   KEYWORD__COMPLEX);
	assert_string_equal(yytext, "_Complex");
	assert_int_equal(yylex(),   KEYWORD__GENERIC);
	assert_string_equal(yytext, "_Generic");
	assert_int_equal(yylex(),   KEYWORD__IMAGINARY);
	assert_string_equal(yytext, "_Imaginary");
	assert_int_equal(yylex(),   KEYWORD__NORETURN);
	assert_string_equal(yytext, "_Noreturn");
	assert_int_equal(yylex(),   KEYWORD__STATIC_ASSERT);
	assert_string_equal(yytext, "_Static_Assert");
	assert_int_equal(yylex(),   KEYWORD__THREAD_LOCAL);
	assert_string_equal(yytext, "_Thread_Local");
}


int main(int argc, char **argv)
{
	(void) argc;

	yyin_next = argv + 1;

	static const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(
			test_identifiers,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_keywords,
			setup,
			teardown
		),
	};


	return cmocka_run_group_tests(tests, NULL, NULL);
}
