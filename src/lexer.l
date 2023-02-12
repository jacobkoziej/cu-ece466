/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * lexer.l -- rudimentary lexer
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

%{
#include <stdio.h>

#include "parser.tab.h"
%}

/* definitions */
SIGN              \+-
DIGIT             [0-9]
NONDIGIT          [_A-Za-z]
NONZERO_DIGIT     [1-9]
OCTAL_DIGIT       [0-7]
HEXADECIMAL_DIGIT [0-9A-Fa-f]
HEX_QUAD          {HEXADECIMAL_DIGIT}{4}
C_CHAR            [^'\\\n]
S_CHAR            [^"\\\n]

OCTAL_ESCAPE_SEQUENCE \\{OCTAL_DIGIT}{1,3}
HEXADECIMAL_ESCAPE_SEQUENCE \\x{HEXADECIMAL_DIGIT}+

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
FRACTIONAL_CONSTANT             ({DIGIT}*\.{DIGIT}+)|({DIGIT}+\.)
HEXADECIMAL_FRACTIONAL_CONSTANT ({HEXADECIMAL_DIGIT}*\.{HEXADECIMAL_DIGIT}+)|({HEXADECIMAL_DIGIT}+\.)

UNIVERSAL_CHARACTER_NAME \\(u{HEX_QUAD}|U{HEX_QUAD}{2})
IDENTIFIER_NONDIGIT      {NONDIGIT}|{UNIVERSAL_CHARACTER_NAME}
IDENTIFIER               {NONDIGIT}({DIGIT}|{IDENTIFIER_NONDIGIT})*

/* start conditions */
%x COMMENT

%%

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
__func__                        return IDENTIFIER___FUNC__;
{NONDIGIT}({DIGIT}|{NONDIGIT})* return IDENTIFIER_GENERAL;
\\u{HEXADECIMAL_DIGIT}{4}       return IDENTIFIER_UNIVERSAL_CHARACTER_NAME;
\\U{HEXADECIMAL_DIGIT}{8}       return IDENTIFIER_UNIVERSAL_CHARACTER_NAME;

              /* comments */
"/*"          BEGIN(COMMENT);
"//".*        ;
<COMMENT>.|\n ;
<COMMENT>"*/" BEGIN(INITIAL);

.|\n ;
