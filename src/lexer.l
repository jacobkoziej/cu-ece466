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
NONDIGIT [_A-Za-z]
DIGIT    [0-9]
HEXDIGIT [0-9A-Fa-f]

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

                                /* identifiers */
__func__                        return IDENTIFIER___FUNC__;
{NONDIGIT}({DIGIT}|{NONDIGIT})* return IDENTIFIER_GENERAL;
\\u{HEXDIGIT}{4}                return IDENTIFIER_UNIVERSAL_CHARACTER_NAME;
\\U{HEXDIGIT}{8}                return IDENTIFIER_UNIVERSAL_CHARACTER_NAME;

.|\n ;
