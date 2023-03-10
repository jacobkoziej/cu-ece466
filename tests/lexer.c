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

#include "lex.yy.h"
#include "y.tab.h"


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


static void test_character_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'c');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'o');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'o');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'p');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'e');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, 'r');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, WCHAR_T);
	assert_int_equal(yylval.character_constant.WCHAR_T, L'\x1f60f');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, CHAR16_T);
	assert_int_equal(yylval.character_constant.CHAR16_T, u'\x267F');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, CHAR32_T);
	assert_int_equal(yylval.character_constant.CHAR32_T, U'\x1f60f');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '"');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '?');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\'');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\"');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\?');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\\');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\a');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\b');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\f');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\n');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\r');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\t');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\v');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\0');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\60');
	assert_int_equal(yylex(), CHARACTER_CONSTANT);
	assert_int_equal(yylval.character_constant.type, UNSIGNED_CHAR);
	assert_int_equal(yylval.character_constant.UNSIGNED_CHAR, '\100');
}

static void test_floating_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 123.456);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 123.);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == .123);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, FLOAT);
	assert_true(yylval.floating_constant.FLOAT == 128.F);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, LONG_DOUBLE);
	assert_true(yylval.floating_constant.LONG_DOUBLE == 128.L);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e+10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10e-10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E+10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 10E-10);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.abbap123);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.abbaP123);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x4b1d.p0);
	assert_int_equal(yylex(), FLOATING_CONSTANT);
	assert_int_equal(yylval.floating_constant.type, DOUBLE);
	assert_true(yylval.floating_constant.DOUBLE == 0x.4b1dp0);
}

static void test_identifiers(void **state)
{
	(void) state;

	assert_int_equal(yylex(), IDENTIFIER);
	assert_string_equal(yylval.string.head, "_start");
	assert_int_equal(yylex(), IDENTIFIER);
	assert_string_equal(yylval.string.head, "__func__");
	assert_int_equal(yylex(), IDENTIFIER);
	assert_string_equal(yylval.string.head, "ascii\u3164unicode");
	assert_int_equal(yylex(), IDENTIFIER);
	assert_string_equal(yylval.string.head, "_13\U0001f60f");
	assert_int_equal(yylex(), IDENTIFIER);
	assert_string_equal(yylval.string.head, "\U0001f60f");
}

static void test_integer_constants(void **state)
{
	(void) state;

	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 1859);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 007);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, INT);
	assert_int_equal(yylval.integer_constant.INT, 0x4b1d);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_INT, 31415U);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_INT, 31415UL);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_LONG_INT, 31415ULL);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, LONG_INT);
	assert_int_equal(yylval.integer_constant.LONG_INT, 31415L);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_INT, 31415LU);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.LONG_LONG_INT, 31415LL);
	assert_int_equal(yylex(), INTEGER_CONSTANT);
	assert_int_equal(yylval.integer_constant.type, UNSIGNED_LONG_LONG_INT);
	assert_int_equal(yylval.integer_constant.UNSIGNED_LONG_LONG_INT, 31415LLU);
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

static void test_punctuators(void **state)
{
	(void) state;
	assert_int_equal(yylex(),   PUNCTUATOR_LBRACKET);
	assert_string_equal(yytext, "[");
	assert_int_equal(yylex(),   PUNCTUATOR_LBRACKET);
	assert_string_equal(yytext, "<:");
	assert_int_equal(yylex(),   PUNCTUATOR_RBRACKET);
	assert_string_equal(yytext, "]");
	assert_int_equal(yylex(),   PUNCTUATOR_RBRACKET);
	assert_string_equal(yytext, ":>");
	assert_int_equal(yylex(),   PUNCTUATOR_LPARENTHESIS);
	assert_string_equal(yytext, "(");
	assert_int_equal(yylex(),   PUNCTUATOR_RPARENTHESIS);
	assert_string_equal(yytext, ")");
	assert_int_equal(yylex(),   PUNCTUATOR_LBRACE);
	assert_string_equal(yytext, "{");
	assert_int_equal(yylex(),   PUNCTUATOR_LBRACE);
	assert_string_equal(yytext, "<%");
	assert_int_equal(yylex(),   PUNCTUATOR_RBRACE);
	assert_string_equal(yytext, "}");
	assert_int_equal(yylex(),   PUNCTUATOR_RBRACE);
	assert_string_equal(yytext, "%>");
	assert_int_equal(yylex(),   PUNCTUATOR_MEMBER_ACCESS);
	assert_string_equal(yytext, ".");
	assert_int_equal(yylex(),   PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE);
	assert_string_equal(yytext, "->");
	assert_int_equal(yylex(),   PUNCTUATOR_INCREMENT);
	assert_string_equal(yytext, "++");
	assert_int_equal(yylex(),   PUNCTUATOR_DECREMENT);
	assert_string_equal(yytext, "--");
	assert_int_equal(yylex(),   PUNCTUATOR_AMPERSAND);
	assert_string_equal(yytext, "&");
	assert_int_equal(yylex(),   PUNCTUATOR_ASTERISK);
	assert_string_equal(yytext, "*");
	assert_int_equal(yylex(),   PUNCTUATOR_PLUS);
	assert_string_equal(yytext, "+");
	assert_int_equal(yylex(),   PUNCTUATOR_MINUS);
	assert_string_equal(yytext, "-");
	assert_int_equal(yylex(),   PUNCTUATOR_UNARY_COMPLEMENT);
	assert_string_equal(yytext, "~");
	assert_int_equal(yylex(),   PUNCTUATOR_LOGICAL_NOT);
	assert_string_equal(yytext, "!");
	assert_int_equal(yylex(),   PUNCTUATOR_DIVISION);
	assert_string_equal(yytext, "/");
	assert_int_equal(yylex(),   PUNCTUATOR_MODULO);
	assert_string_equal(yytext, "%");
	assert_int_equal(yylex(),   PUNCTUATOR_LBITSHIFT);
	assert_string_equal(yytext, "<<");
	assert_int_equal(yylex(),   PUNCTUATOR_RBITSHIFT);
	assert_string_equal(yytext, ">>");
	assert_int_equal(yylex(),   PUNCTUATOR_LESS_THAN);
	assert_string_equal(yytext, "<");
	assert_int_equal(yylex(),   PUNCTUATOR_GREATER_THAN);
	assert_string_equal(yytext, ">");
	assert_int_equal(yylex(),   PUNCTUATOR_LESS_THAN_OR_EQUAL);
	assert_string_equal(yytext, "<=");
	assert_int_equal(yylex(),   PUNCTUATOR_GREATER_THAN_OR_EQUAL);
	assert_string_equal(yytext, ">=");
	assert_int_equal(yylex(),   PUNCTUATOR_EQUALITY);
	assert_string_equal(yytext, "==");
	assert_int_equal(yylex(),   PUNCTUATOR_INEQUALITY);
	assert_string_equal(yytext, "!=");
	assert_int_equal(yylex(),   PUNCTUATOR_XOR);
	assert_string_equal(yytext, "^");
	assert_int_equal(yylex(),   PUNCTUATOR_OR);
	assert_string_equal(yytext, "|");
	assert_int_equal(yylex(),   PUNCTUATOR_LOGICAL_AND);
	assert_string_equal(yytext, "&&");
	assert_int_equal(yylex(),   PUNCTUATOR_LOGICAL_OR);
	assert_string_equal(yytext, "||");
	assert_int_equal(yylex(),   PUNCTUATOR_CONDITIONAL_QUESTION);
	assert_string_equal(yytext, "?");
	assert_int_equal(yylex(),   PUNCTUATOR_CONDITIONAL_COLON);
	assert_string_equal(yytext, ":");
	assert_int_equal(yylex(),   PUNCTUATOR_SEMICOLON);
	assert_string_equal(yytext, ";");
	assert_int_equal(yylex(),   PUNCTUATOR_VARIADIC);
	assert_string_equal(yytext, "...");
	assert_int_equal(yylex(),   PUNCTUATOR_ASSIGNMENT);
	assert_string_equal(yytext, "=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_MULTIPLICATION);
	assert_string_equal(yytext, "*=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_DIVISION);
	assert_string_equal(yytext, "/=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_MODULO);
	assert_string_equal(yytext, "%=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_ADDITION);
	assert_string_equal(yytext, "+=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_SUBTRACTION);
	assert_string_equal(yytext, "-=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_LBITSHIFT);
	assert_string_equal(yytext, "<<=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_RBITSHIFT);
	assert_string_equal(yytext, ">>=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_AND);
	assert_string_equal(yytext, "&=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_XOR);
	assert_string_equal(yytext, "^=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMPOUND_OR);
	assert_string_equal(yytext, "|=");
	assert_int_equal(yylex(),   PUNCTUATOR_COMMA);
	assert_string_equal(yytext, ",");
	assert_int_equal(yylex(),   PUNCTUATOR_PREPROCESSOR);
	assert_string_equal(yytext, "#");
	assert_int_equal(yylex(),   PUNCTUATOR_PREPROCESSOR);
	assert_string_equal(yytext, "%:");
	assert_int_equal(yylex(),   PUNCTUATOR_PREPROCESSOR_PASTING);
	assert_string_equal(yytext, "##");
	assert_int_equal(yylex(),   PUNCTUATOR_PREPROCESSOR_PASTING);
	assert_string_equal(yytext, "%:%:");
}

static void test_string_literals(void **state)
{
	(void) state;

	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_UTF_8);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR16_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR32_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_WCHAR_T);
	assert_string_equal(yylval.string_literal.string.head, "i love the cooper");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "'");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "?");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\'");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\"");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\?");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\\");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\a");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\b");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\f");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\n");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\r");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\t");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\v");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\0");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\60");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\100");
	assert_int_equal(yylex(), STRING_LITERAL);
	assert_int_equal(yylval.string_literal.type, STRING_CHAR);
	assert_string_equal(yylval.string_literal.string.head, "\xf");
	assert_int_equal(yylex(), STRING_LITERAL);
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
