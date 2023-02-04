// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.y -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

%{
void yyerror(char const *token)
{
	(void) token;
}
%}

%token KEYWORD_AUTO
%token KEYWORD_BREAK
%token KEYWORD_CASE
%token KEYWORD_CHAR
%token KEYWORD_CONST
%token KEYWORD_CONTINUE
%token KEYWORD_DEFAULT
%token KEYWORD_DO
%token KEYWORD_DOUBLE
%token KEYWORD_ELSE
%token KEYWORD_ENUM
%token KEYWORD_EXTERN
%token KEYWORD_FLOAT
%token KEYWORD_FOR
%token KEYWORD_GOTO
%token KEYWORD_IF
%token KEYWORD_INLINE
%token KEYWORD_INT
%token KEYWORD_LONG
%token KEYWORD_REGISTER
%token KEYWORD_RESTRICT
%token KEYWORD_RETURN
%token KEYWORD_SHORT
%token KEYWORD_SIGNED
%token KEYWORD_SIZEOF
%token KEYWORD_STATIC
%token KEYWORD_STRUCT
%token KEYWORD_SWITCH
%token KEYWORD_TYPEDEF
%token KEYWORD_UNION
%token KEYWORD_UNSIGNED
%token KEYWORD_VOID
%token KEYWORD_VOLATILE
%token KEYWORD_WHILE
%token KEYWORD__ALIGNAS
%token KEYWORD__ALIGNOF
%token KEYWORD__ATOMIC
%token KEYWORD__BOOL
%token KEYWORD__COMPLEX
%token KEYWORD__GENERIC
%token KEYWORD__IMAGINARY
%token KEYWORD__NORETURN
%token KEYWORD__STATIC_ASSERT
%token KEYWORD__THREAD_LOCAL

%token IDENTIFIER_NONDIGIT
%token IDENTIFIER_DIGIT
%token IDENTIFIER___FUNC__

%token UNIVERSAL_CHARACTER_NAME

%%

todo: // TODO: add rules
