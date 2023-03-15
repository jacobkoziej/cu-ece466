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

#include <jkcc/lexer.h>

#include "y.tab.h"
#include "lex.yy.h"


static char      **yyin_next;
static FILE       *yyin;
static yyscan_t    yyscanner;
static YYSTYPE     yylval;
static YYLTYPE     yylloc;
static file_t      yyfile;
static yyextra_t   yyextra_data = {
	.file = &yyfile,
};


static int setup(void **state)
{
	(void) state;

	yyfile.path = *yyin_next;

	yylex_init(&yyscanner);
	yylex_init_extra(&yyextra_data, yyscanner);

	yyin = fopen(*yyin_next++, "r");
	yyrestart(yyin, yyscanner);

	return 0;
}

static int teardown(void **state)
{
	(void) state;

	fclose(yyin);
	yylex_destroy(yyscanner);

	return 0;
}


static void test_character_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'c');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'o');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'o');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'p');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'e');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'r');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, WCHAR_T);
	assert_int_equal(yylval.character_constant.WCHAR_T, L'\x1f60f');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, CHAR16_T);
	assert_int_equal(yylval.character_constant.CHAR16_T, u'\x267F');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, CHAR32_T);
	assert_int_equal(yylval.character_constant.CHAR32_T, U'\x1f60f');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '"');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '?');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\'');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\"');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\?');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\\');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\a');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\b');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\f');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\n');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\r');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\t');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\v');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\0');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\60');
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\100');
}

static void test_floating_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 123.456);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 123.);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == .123);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, FLOAT);
	assert_true(yylval.floating_constant.FLOAT == 128.F);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, LONG_DOUBLE);
	assert_true(yylval.floating_constant.LONG_DOUBLE == 128.L);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e+10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e-10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E+10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E-10);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.abbap123);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.abbaP123);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.p0);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x.4b1dp0);
}

static void test_identifiers(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), IDENTIFIER);
	assert_string_equal(yylval.identifier.IDENTIFIER.head, "_start");
	assert_string_equal(yylval.identifier.text.head, "_start");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), IDENTIFIER);
	assert_string_equal(yylval.identifier.IDENTIFIER.head, "__func__");
	assert_string_equal(yylval.identifier.text.head, "__func__");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), IDENTIFIER);
	assert_string_equal(yylval.identifier.IDENTIFIER.head, "ascii\u3164unicode");
	assert_string_equal(yylval.identifier.text.head, "ascii\\u3164unicode");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), IDENTIFIER);
	assert_string_equal(yylval.identifier.IDENTIFIER.head, "_13\U0001f60f");
	assert_string_equal(yylval.identifier.text.head, "_13\\U0001f60f");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), IDENTIFIER);
	assert_string_equal(yylval.identifier.IDENTIFIER.head, "\U0001f60f");
	assert_string_equal(yylval.identifier.text.head, "\\U0001f60f");
}

static void test_integer_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 1859);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 007);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 0x4b1d);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_INT, 31415U);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_INT, 31415UL);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_LONG_INT, 31415ULL);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, LONG_INT);
	assert_int_equal(yylval.integer_constant.LONG_INT, 31415L);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_INT, 31415LU);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.LONG_LONG_INT, 31415LL);
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_LONG_INT, 31415LLU);
}

static void test_keywords(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_AUTO);
	assert_string_equal(yyget_text(yyscanner), "auto");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_BREAK);
	assert_string_equal(yyget_text(yyscanner), "break");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_CASE);
	assert_string_equal(yyget_text(yyscanner), "case");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_CHAR);
	assert_string_equal(yyget_text(yyscanner), "char");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_CONST);
	assert_string_equal(yyget_text(yyscanner), "const");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_CONTINUE);
	assert_string_equal(yyget_text(yyscanner), "continue");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_DEFAULT);
	assert_string_equal(yyget_text(yyscanner), "default");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_DO);
	assert_string_equal(yyget_text(yyscanner), "do");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_DOUBLE);
	assert_string_equal(yyget_text(yyscanner), "double");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_ELSE);
	assert_string_equal(yyget_text(yyscanner), "else");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_ENUM);
	assert_string_equal(yyget_text(yyscanner), "enum");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_EXTERN);
	assert_string_equal(yyget_text(yyscanner), "extern");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_FLOAT);
	assert_string_equal(yyget_text(yyscanner), "float");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_FOR);
	assert_string_equal(yyget_text(yyscanner), "for");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_GOTO);
	assert_string_equal(yyget_text(yyscanner), "goto");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_IF);
	assert_string_equal(yyget_text(yyscanner), "if");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_INLINE);
	assert_string_equal(yyget_text(yyscanner), "inline");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_INT);
	assert_string_equal(yyget_text(yyscanner), "int");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_LONG);
	assert_string_equal(yyget_text(yyscanner), "long");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_REGISTER);
	assert_string_equal(yyget_text(yyscanner), "register");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_RESTRICT);
	assert_string_equal(yyget_text(yyscanner), "restrict");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_RETURN);
	assert_string_equal(yyget_text(yyscanner), "return");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_SHORT);
	assert_string_equal(yyget_text(yyscanner), "short");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_SIGNED);
	assert_string_equal(yyget_text(yyscanner), "signed");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_SIZEOF);
	assert_string_equal(yyget_text(yyscanner), "sizeof");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_STATIC);
	assert_string_equal(yyget_text(yyscanner), "static");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_STRUCT);
	assert_string_equal(yyget_text(yyscanner), "struct");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_SWITCH);
	assert_string_equal(yyget_text(yyscanner), "switch");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_TYPEDEF);
	assert_string_equal(yyget_text(yyscanner), "typedef");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_UNION);
	assert_string_equal(yyget_text(yyscanner), "union");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_UNSIGNED);
	assert_string_equal(yyget_text(yyscanner), "unsigned");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_VOID);
	assert_string_equal(yyget_text(yyscanner), "void");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_VOLATILE);
	assert_string_equal(yyget_text(yyscanner), "volatile");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD_WHILE);
	assert_string_equal(yyget_text(yyscanner), "while");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__ALIGNAS);
	assert_string_equal(yyget_text(yyscanner), "_Alignas");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__ALIGNOF);
	assert_string_equal(yyget_text(yyscanner), "_Alignof");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__ATOMIC);
	assert_string_equal(yyget_text(yyscanner), "_Atomic");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__BOOL);
	assert_string_equal(yyget_text(yyscanner), "_Bool");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__COMPLEX);
	assert_string_equal(yyget_text(yyscanner), "_Complex");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__GENERIC);
	assert_string_equal(yyget_text(yyscanner), "_Generic");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__IMAGINARY);
	assert_string_equal(yyget_text(yyscanner), "_Imaginary");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__NORETURN);
	assert_string_equal(yyget_text(yyscanner), "_Noreturn");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__STATIC_ASSERT);
	assert_string_equal(yyget_text(yyscanner), "_Static_Assert");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), KEYWORD__THREAD_LOCAL);
	assert_string_equal(yyget_text(yyscanner), "_Thread_Local");
}

static void test_punctuators(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LBRACKET);
	assert_string_equal(yyget_text(yyscanner), "[");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LBRACKET);
	assert_string_equal(yyget_text(yyscanner), "<:");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RBRACKET);
	assert_string_equal(yyget_text(yyscanner), "]");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RBRACKET);
	assert_string_equal(yyget_text(yyscanner), ":>");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LPARENTHESIS);
	assert_string_equal(yyget_text(yyscanner), "(");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RPARENTHESIS);
	assert_string_equal(yyget_text(yyscanner), ")");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LBRACE);
	assert_string_equal(yyget_text(yyscanner), "{");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LBRACE);
	assert_string_equal(yyget_text(yyscanner), "<%");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RBRACE);
	assert_string_equal(yyget_text(yyscanner), "}");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RBRACE);
	assert_string_equal(yyget_text(yyscanner), "%>");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_MEMBER_ACCESS);
	assert_string_equal(yyget_text(yyscanner), ".");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE);
	assert_string_equal(yyget_text(yyscanner), "->");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_INCREMENT);
	assert_string_equal(yyget_text(yyscanner), "++");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_DECREMENT);
	assert_string_equal(yyget_text(yyscanner), "--");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_AMPERSAND);
	assert_string_equal(yyget_text(yyscanner), "&");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_ASTERISK);
	assert_string_equal(yyget_text(yyscanner), "*");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_PLUS);
	assert_string_equal(yyget_text(yyscanner), "+");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_MINUS);
	assert_string_equal(yyget_text(yyscanner), "-");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_UNARY_COMPLEMENT);
	assert_string_equal(yyget_text(yyscanner), "~");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LOGICAL_NOT);
	assert_string_equal(yyget_text(yyscanner), "!");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_DIVISION);
	assert_string_equal(yyget_text(yyscanner), "/");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_MODULO);
	assert_string_equal(yyget_text(yyscanner), "%");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LBITSHIFT);
	assert_string_equal(yyget_text(yyscanner), "<<");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_RBITSHIFT);
	assert_string_equal(yyget_text(yyscanner), ">>");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LESS_THAN);
	assert_string_equal(yyget_text(yyscanner), "<");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_GREATER_THAN);
	assert_string_equal(yyget_text(yyscanner), ">");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LESS_THAN_OR_EQUAL);
	assert_string_equal(yyget_text(yyscanner), "<=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_GREATER_THAN_OR_EQUAL);
	assert_string_equal(yyget_text(yyscanner), ">=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_EQUALITY);
	assert_string_equal(yyget_text(yyscanner), "==");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_INEQUALITY);
	assert_string_equal(yyget_text(yyscanner), "!=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_XOR);
	assert_string_equal(yyget_text(yyscanner), "^");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_OR);
	assert_string_equal(yyget_text(yyscanner), "|");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LOGICAL_AND);
	assert_string_equal(yyget_text(yyscanner), "&&");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_LOGICAL_OR);
	assert_string_equal(yyget_text(yyscanner), "||");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_CONDITIONAL_QUESTION);
	assert_string_equal(yyget_text(yyscanner), "?");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_CONDITIONAL_COLON);
	assert_string_equal(yyget_text(yyscanner), ":");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_SEMICOLON);
	assert_string_equal(yyget_text(yyscanner), ";");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_VARIADIC);
	assert_string_equal(yyget_text(yyscanner), "...");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_ASSIGNMENT);
	assert_string_equal(yyget_text(yyscanner), "=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_MULTIPLICATION);
	assert_string_equal(yyget_text(yyscanner), "*=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_DIVISION);
	assert_string_equal(yyget_text(yyscanner), "/=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_MODULO);
	assert_string_equal(yyget_text(yyscanner), "%=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_ADDITION);
	assert_string_equal(yyget_text(yyscanner), "+=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_SUBTRACTION);
	assert_string_equal(yyget_text(yyscanner), "-=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_LBITSHIFT);
	assert_string_equal(yyget_text(yyscanner), "<<=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_RBITSHIFT);
	assert_string_equal(yyget_text(yyscanner), ">>=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_AND);
	assert_string_equal(yyget_text(yyscanner), "&=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_XOR);
	assert_string_equal(yyget_text(yyscanner), "^=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMPOUND_OR);
	assert_string_equal(yyget_text(yyscanner), "|=");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_COMMA);
	assert_string_equal(yyget_text(yyscanner), ",");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_PREPROCESSOR);
	assert_string_equal(yyget_text(yyscanner), "#");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_PREPROCESSOR);
	assert_string_equal(yyget_text(yyscanner), "%:");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_PREPROCESSOR_PASTING);
	assert_string_equal(yyget_text(yyscanner), "##");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), PUNCTUATOR_PREPROCESSOR_PASTING);
	assert_string_equal(yyget_text(yyscanner), "%:%:");
}

static void test_string_literals(void **state)
{
	(void) state;

	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_UTF_8);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR16_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR32_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_WCHAR_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "'");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "?");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\'");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\"");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\?");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\\");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\a");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\b");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\f");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\n");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\r");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\t");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\v");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\0");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\60");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\100");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\xf");
	assert_int_equal(yylex(&yylval, &yylloc, yyscanner), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\xff");
}


int main(int argc, char **argv)
{
	(void) argc;

	yyin_next = argv + 1;

	static const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(
			test_character_constants,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_floating_constants,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_identifiers,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_integer_constants,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_keywords,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_punctuators,
			setup,
			teardown
		),
		cmocka_unit_test_setup_teardown(
			test_string_literals,
			setup,
			teardown
		),
	};


	return cmocka_run_group_tests(tests, NULL, NULL);
}
