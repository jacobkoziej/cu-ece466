/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * lexer.l -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

%{
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>
#include <jkcc/string.h>

#include "y.tab.h"


#define M_INTEGER_CONSTANT(func, start, end, type) {\
	*(end) = '\0';\
	int ret = (func)(\
		(start),\
		(end),\
		&yylval.integer_constant,\
		integer_constant_base,\
		(type));\
	BEGIN(INITIAL);\
	return (ret < 0) ? YYerror : INTEGER_CONSTANT;\
}

#define M_CHARACTER_CONSTANT(val) {\
	yylval.character_constant.type = character_constant_type;\
	switch (character_constant_type) {\
		case UNSIGNED_CHAR:\
			yylval.character_constant.UNSIGNED_CHAR = (val);\
			break;\
		case WCHAR_T:\
			yylval.character_constant.WCHAR_T = (val);\
			break;\
		case CHAR16_T:\
			yylval.character_constant.CHAR16_T = (val);\
			break;\
		case CHAR32_T:\
			yylval.character_constant.CHAR32_T = (val);\
			break;\
	}\
	BEGIN(INITIAL);\
	return CHARACTER_CONSTANT;\
}

#define M_CHARACTER_CONSTANT_ESCAPE_SEQUENCE(start, end, base) {\
	*(end) = '\0';\
	int ret = lexer_character_constant(\
		(start),\
		(end),\
		&yylval.character_constant,\
		(base),\
		character_constant_type);\
	BEGIN(INITIAL);\
	return (ret < 0) ? YYerror : CHARACTER_CONSTANT;\
}

#define M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE(val) {\
	if (string_append(&yylval.string_literal.string, (val), 1) < 0)\
		return YYerror;\
}


static int                       integer_constant_base;
static enum character_constant_e character_constant_type;
%}


/* definitions */
SIGN              [\+-]
DIGIT             [0-9]
NONDIGIT          [_A-Za-z]
NONZERO_DIGIT     [1-9]
OCTAL_DIGIT       [0-7]
HEXADECIMAL_DIGIT [0-9A-Fa-f]
HEX_QUAD          {HEXADECIMAL_DIGIT}{4}
C_CHAR            [^'\\\n]
S_CHAR            [^"\\\n]

DIGIT_SEQUENCE              {DIGIT}+
HEXADECIMAL_DIGIT_SEQUENCE  {HEXADECIMAL_DIGIT}+
OCTAL_ESCAPE_SEQUENCE       \\{OCTAL_DIGIT}{1,3}
HEXADECIMAL_ESCAPE_SEQUENCE \\x{HEXADECIMAL_DIGIT}+
UNIVERSAL_CHARACTER_NAME    \\(u{HEX_QUAD}|U{HEX_QUAD}{2})

HEXADECIMAL_PREFIX   0[xX]
UNSIGNED_SUFFIX      [uU]
LONG_SUFFIX          [lL]
LONG_LONG_SUFFIX     ll|LL
FLOATING_SUFFIX      [flFL]
EXPONENT_PART        [eE]{SIGN}?{DIGIT}+
BINARY_EXPONENT_PART [pP]{SIGN}?{DIGIT}+

DECIMAL_CONSTANT                {NONZERO_DIGIT}{DIGIT}*
OCTAL_CONSTANT                  0{OCTAL_DIGIT}*
HEXADECIMAL_CONSTANT            {HEXADECIMAL_PREFIX}{HEXADECIMAL_DIGIT}*
FRACTIONAL_CONSTANT             ({DIGIT_SEQUENCE}?\.{DIGIT_SEQUENCE})|({DIGIT_SEQUENCE}\.)
HEXADECIMAL_FRACTIONAL_CONSTANT ({HEXADECIMAL_DIGIT_SEQUENCE}?\.{HEXADECIMAL_DIGIT_SEQUENCE})|({HEXADECIMAL_DIGIT_SEQUENCE}\.)


/* start conditions */
%x SC_IDENTIFIER
%x SC_INTEGER_CONSTANT
%x SC_FLOATING_CONSTANT
%x SC_CHARACTER_CONSTANT
%x SC_STRING_LITERAL
%x SC_COMMENT


%%


%{
	(void) input;
%}


	/* keywords */
auto           return KEYWORD_AUTO;
break          return KEYWORD_BREAK;
case           return KEYWORD_CASE;
char           return KEYWORD_CHAR;
const          return KEYWORD_CONST;
continue       return KEYWORD_CONTINUE;
default        return KEYWORD_DEFAULT;
do             return KEYWORD_DO;
double         return KEYWORD_DOUBLE;
else           return KEYWORD_ELSE;
enum           return KEYWORD_ENUM;
extern         return KEYWORD_EXTERN;
float          return KEYWORD_FLOAT;
for            return KEYWORD_FOR;
goto           return KEYWORD_GOTO;
if             return KEYWORD_IF;
inline         return KEYWORD_INLINE;
int            return KEYWORD_INT;
long           return KEYWORD_LONG;
register       return KEYWORD_REGISTER;
restrict       return KEYWORD_RESTRICT;
return         return KEYWORD_RETURN;
short          return KEYWORD_SHORT;
signed         return KEYWORD_SIGNED;
sizeof         return KEYWORD_SIZEOF;
static         return KEYWORD_STATIC;
struct         return KEYWORD_STRUCT;
switch         return KEYWORD_SWITCH;
typedef        return KEYWORD_TYPEDEF;
union          return KEYWORD_UNION;
unsigned       return KEYWORD_UNSIGNED;
void           return KEYWORD_VOID;
volatile       return KEYWORD_VOLATILE;
while          return KEYWORD_WHILE;
_Alignas       return KEYWORD__ALIGNAS;
_Alignof       return KEYWORD__ALIGNOF;
_Atomic        return KEYWORD__ATOMIC;
_Bool          return KEYWORD__BOOL;
_Complex       return KEYWORD__COMPLEX;
_Generic       return KEYWORD__GENERIC;
_Imaginary     return KEYWORD__IMAGINARY;
_Noreturn      return KEYWORD__NORETURN;
_Static_Assert return KEYWORD__STATIC_ASSERT;
_Thread_Local  return KEYWORD__THREAD_LOCAL;


	/* punctuators */
"["|"<:"    return PUNCTUATOR_LBRACKET;
"]"|":>"    return PUNCTUATOR_RBRACKET;
"("         return PUNCTUATOR_LPARENTHESIS;
")"         return PUNCTUATOR_RPARENTHESIS;
"{"|"<%"    return PUNCTUATOR_LBRACE;
"}"|"%>"    return PUNCTUATOR_RBRACE;
"."         return PUNCTUATOR_MEMBER_ACCESS;
"->"        return PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE;
"++"        return PUNCTUATOR_INCREMENT;
"--"        return PUNCTUATOR_DECREMENT;
"&"         return PUNCTUATOR_AMPERSAND;
"*"         return PUNCTUATOR_ASTERISK;
"+"         return PUNCTUATOR_PLUS;
"-"         return PUNCTUATOR_MINUS;
"~"         return PUNCTUATOR_UNARY_COMPLEMENT;
"!"         return PUNCTUATOR_LOGICAL_NOT;
"/"         return PUNCTUATOR_DIVISION;
"%"         return PUNCTUATOR_MODULO;
"<<"        return PUNCTUATOR_LBITSHIFT;
">>"        return PUNCTUATOR_RBITSHIFT;
"<"         return PUNCTUATOR_LESS_THAN;
">"         return PUNCTUATOR_GREATER_THAN;
"<="        return PUNCTUATOR_LESS_THAN_OR_EQUAL;
">="        return PUNCTUATOR_GREATER_THAN_OR_EQUAL;
"=="        return PUNCTUATOR_EQUALITY;
"!="        return PUNCTUATOR_INEQUALITY;
"^"         return PUNCTUATOR_XOR;
"|"         return PUNCTUATOR_OR;
"&&"        return PUNCTUATOR_LOGICAL_AND;
"||"        return PUNCTUATOR_LOGICAL_OR;
"?"         return PUNCTUATOR_CONDITIONAL_QUESTION;
":"         return PUNCTUATOR_CONDITIONAL_COLON;
";"         return PUNCTUATOR_SEMICOLON;
"..."       return PUNCTUATOR_VARIADIC;
"="         return PUNCTUATOR_ASSIGNMENT;
"*="        return PUNCTUATOR_COMPOUND_MULTIPLICATION;
"/="        return PUNCTUATOR_COMPOUND_DIVISION;
"%="        return PUNCTUATOR_COMPOUND_MODULO;
"+="        return PUNCTUATOR_COMPOUND_ADDITION;
"-="        return PUNCTUATOR_COMPOUND_SUBTRACTION;
"<<="       return PUNCTUATOR_COMPOUND_LBITSHIFT;
">>="       return PUNCTUATOR_COMPOUND_RBITSHIFT;
"&="        return PUNCTUATOR_COMPOUND_AND;
"^="        return PUNCTUATOR_COMPOUND_XOR;
"|="        return PUNCTUATOR_COMPOUND_OR;
","         return PUNCTUATOR_COMMA;
"#"|"%:"    return PUNCTUATOR_PREPROCESSOR;
"##"|"%:%:" return PUNCTUATOR_PREPROCESSOR_PASTING;


	/* identifiers */
{NONDIGIT} {
	if (string_init(&yylval.string, 0) < 0) return YYerror;

	if (string_append(&yylval.string, yytext, yyleng) < 0) return YYerror;

	BEGIN(SC_IDENTIFIER);
}

{UNIVERSAL_CHARACTER_NAME} {
	if (string_init(&yylval.string, 0) < 0) return YYerror;

	uint32_t ucn;

	int ret = lexer_universal_character_name(
		yytext + 2,
		yytext + yyleng - 1,
		&ucn);

	if (ret < 0) return YYerror;

	char   buf[4];
	size_t bytes;

	lexer_utf32_to_utf8(buf, &bytes, ucn);

	if (string_append(&yylval.string, buf, bytes) < 0) return YYerror;

	BEGIN(SC_IDENTIFIER);
}

<SC_IDENTIFIER>{
({DIGIT}|{NONDIGIT})* {
	if (string_append(&yylval.string, yytext, yyleng) < 0) return YYerror;
}

{UNIVERSAL_CHARACTER_NAME} {
	uint32_t ucn;

	int ret = lexer_universal_character_name(
		yytext + 2,
		yytext + yyleng - 1,
		&ucn);

	if (ret < 0) return YYerror;

	char   buf[4];
	size_t bytes;

	lexer_utf32_to_utf8(buf, &bytes, ucn);

	if (string_append(&yylval.string, buf, bytes) < 0) return YYerror;
}

.|\n {
	unput(*yytext);
	BEGIN(INITIAL);
	return IDENTIFIER;
}
}


	/* integer constants */
{DECIMAL_CONSTANT} {
	integer_constant_base = 10;
	yymore();
	BEGIN(SC_INTEGER_CONSTANT);
}

{OCTAL_CONSTANT} {
	integer_constant_base = 8;
	yymore();
	BEGIN(SC_INTEGER_CONSTANT);
}

{HEXADECIMAL_CONSTANT} {
	integer_constant_base = 16;
	yymore();
	BEGIN(SC_INTEGER_CONSTANT);
}

<SC_INTEGER_CONSTANT>{
{UNSIGNED_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_unsigned_integer_constant,
		yytext,
		yytext + yyleng - 1,
		0);
}

{UNSIGNED_SUFFIX}{LONG_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_unsigned_integer_constant,
		yytext,
		yytext + yyleng - 2,
		UNSIGNED_LONG_INT);
}

{UNSIGNED_SUFFIX}{LONG_LONG_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_unsigned_integer_constant,
		yytext,
		yytext + yyleng - 3,
		UNSIGNED_LONG_LONG_INT);
}

{LONG_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_signed_integer_constant,
		yytext,
		yytext + yyleng - 1,
		LONG_INT);
}

{LONG_SUFFIX}{UNSIGNED_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_unsigned_integer_constant,
		yytext,
		yytext + yyleng - 2,
		UNSIGNED_LONG_INT);
}

{LONG_LONG_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_signed_integer_constant,
		yytext,
		yytext + yyleng - 2,
		LONG_LONG_INT);
}

{LONG_LONG_SUFFIX}{UNSIGNED_SUFFIX} {
	M_INTEGER_CONSTANT(
		lexer_unsigned_integer_constant,
		yytext,
		yytext + yyleng - 3,
		UNSIGNED_LONG_LONG_INT);
}

.|\n {
	char tmp = yytext[yyleng - 1];
	yytext[yyleng - 1] = '\0';

	int ret = lexer_signed_integer_constant(
		yytext,
		NULL,
		&yylval.integer_constant,
		integer_constant_base,
		0);

	unput(tmp);
	BEGIN(INITIAL);
	return (ret < 0) ? YYerror : INTEGER_CONSTANT;
}
}


	/* floating constants */
{FRACTIONAL_CONSTANT}{EXPONENT_PART}?                                                              |
{DIGIT}+{EXPONENT_PART}                                                                            |
{HEXADECIMAL_PREFIX}({HEXADECIMAL_FRACTIONAL_CONSTANT}|{HEXADECIMAL_DIGIT}+){BINARY_EXPONENT_PART} {
	yymore();
	BEGIN(SC_FLOATING_CONSTANT);
}

<SC_FLOATING_CONSTANT>{
{FLOATING_SUFFIX} {
	char suffix = yytext[yyleng - 1];

	enum floating_constant_e type = ((suffix == 'f') || (suffix == 'F'))
		? FLOAT
		: LONG_DOUBLE;

	yytext[yyleng - 1] = '\0';

	int ret = lexer_floating_constant(
		yytext,
		NULL,
		&yylval.floating_constant,
		type);

	BEGIN(INITIAL);
	return (ret < 0) ? YYerror : FLOATING_CONSTANT;
}

.|\n {
	char tmp = yytext[yyleng - 1];
	yytext[yyleng - 1] = '\0';

	int ret = lexer_floating_constant(
		yytext,
		NULL,
		&yylval.floating_constant,
		DOUBLE);

	unput(tmp);
	BEGIN(INITIAL);
	return (ret < 0) ? YYerror : FLOATING_CONSTANT;
}
}


	/* character constants */
' {
	character_constant_type = UNSIGNED_CHAR;
	BEGIN(SC_CHARACTER_CONSTANT);
}

L' {
	character_constant_type = WCHAR_T;
	BEGIN(SC_CHARACTER_CONSTANT);
}

u' {
	character_constant_type = CHAR16_T;
	BEGIN(SC_CHARACTER_CONSTANT);
}

U' {
	character_constant_type = CHAR32_T;
	BEGIN(SC_CHARACTER_CONSTANT);
}

<SC_CHARACTER_CONSTANT>{
\\''      M_CHARACTER_CONSTANT('\'');
\\\"'     M_CHARACTER_CONSTANT('\"');
\\\?'     M_CHARACTER_CONSTANT('\?');
\\\\'     M_CHARACTER_CONSTANT('\\');
\\a'      M_CHARACTER_CONSTANT('\a');
\\b'      M_CHARACTER_CONSTANT('\b');
\\f'      M_CHARACTER_CONSTANT('\f');
\\n'      M_CHARACTER_CONSTANT('\n');
\\r'      M_CHARACTER_CONSTANT('\r');
\\t'      M_CHARACTER_CONSTANT('\t');
\\v'      M_CHARACTER_CONSTANT('\v');
{C_CHAR}' M_CHARACTER_CONSTANT(*yytext);

{OCTAL_ESCAPE_SEQUENCE}' {
	M_CHARACTER_CONSTANT_ESCAPE_SEQUENCE(
		yytext + 1,
		yytext + yyleng - 1,
		8);
}

{HEXADECIMAL_ESCAPE_SEQUENCE}' {
	M_CHARACTER_CONSTANT_ESCAPE_SEQUENCE(
		yytext + 2,
		yytext + yyleng - 1,
		16);
}

.|\n {
	unput(*yytext);
	BEGIN(INITIAL);
	return YYerror;
}
}


	/* string literals */
\" {
	if (string_init(&yylval.string_literal.string, 0) < 0) return YYerror;

	yylval.string_literal.type = STRING_CHAR;
	BEGIN(SC_STRING_LITERAL);
}

u8\" {
	if (string_init(&yylval.string_literal.string, 0) < 0) return YYerror;

	yylval.string_literal.type = STRING_UTF_8;
	BEGIN(SC_STRING_LITERAL);
}

u\" {
	if (string_init(&yylval.string_literal.string, 0) < 0) return YYerror;

	yylval.string_literal.type = STRING_CHAR16_T;
	BEGIN(SC_STRING_LITERAL);
}

U\" {
	if (string_init(&yylval.string_literal.string, 0) < 0) return YYerror;

	yylval.string_literal.type = STRING_CHAR32_T;
	BEGIN(SC_STRING_LITERAL);
}

L\" {
	if (string_init(&yylval.string_literal.string, 0) < 0) return YYerror;

	yylval.string_literal.type = STRING_WCHAR_T;
	BEGIN(SC_STRING_LITERAL);
}

<SC_STRING_LITERAL>{
\\'      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\'");
\\\"     M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\"");
\\\?     M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\?");
\\\\     M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\\");
\\a      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\a");
\\b      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\b");
\\f      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\f");
\\n      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\n");
\\r      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\r");
\\t      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\t");
\\v      M_STRING_LITERAL_SIMPLE_ESCAPE_SEQUENCE("\v");

{S_CHAR}+ {
	if (string_append(&yylval.string_literal.string, yytext, yyleng) < 0)
		return YYerror;
}

{OCTAL_ESCAPE_SEQUENCE} {
	errno = 0;
	unsigned long long int tmp = strtoull(yytext + 1, NULL, 8);
	if (errno) return YYerror;

	char   buf[2];
	size_t pos = 0;

	if (tmp > UCHAR_MAX) buf[pos++] = tmp >> 8;
	buf[pos] = tmp;

	if (string_append(&yylval.string_literal.string, buf, pos + 1) < 0)
		return YYerror;
}

{HEXADECIMAL_ESCAPE_SEQUENCE} {
	char *start = yytext + 2;

	char tmp;
	char byte;

	// deal with nibble
	if ((yyleng - 2) % 2) {
		tmp = start[1];
		start[1] = '\0';

		errno = 0;
		byte = strtoull(start, NULL, 16);
		if (errno) return YYerror;
		printf("%u\n", byte);

		if (string_append(&yylval.string_literal.string, &byte, 1) < 0)
			return YYerror;

		start[1] = tmp;
		++start;
	}

	while (*start) {
		tmp = start[2];
		start[2] = '\0';

		errno = 0;
		byte = strtoull(start, NULL, 16);
		if (errno) return YYerror;

		if (string_append(&yylval.string_literal.string, &byte, 1) < 0)
			return YYerror;

		start[2] = tmp;
		start += 2;
	}
}

\" {
	BEGIN(INITIAL);
	return STRING_LITERAL;
}

.|\n {
	unput(*yytext);
	BEGIN(INITIAL);
	return YYerror;
}
}


	/* comments */
"//".* ;
"/*"   BEGIN(SC_COMMENT);

<SC_COMMENT>{
[^*]*|\n    ;
"*"+[^/]|\n ;
"*"+"/"     BEGIN(INITIAL);
}


	/* catch all */
.|\n ;
