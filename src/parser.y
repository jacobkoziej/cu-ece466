// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.y -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

%define api.pure full
%locations


%{
#include "y.tab.h"
#include "lex.yy.h"

#include <jkcc/lexer.h>
#include <jkcc/string.h>


#define YYLLOC_DEFAULT(cur, rhs, n) {\
	if (n) {\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.offset;\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.line;\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.column;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.offset;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.line;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.column;\
	} else {\
		(cur).start.offset = (cur).end.offset = YYRHSLOC(rhs, 0).end.offset;\
		(cur).start.line   = (cur).end.line   = YYRHSLOC(rhs, 0).end.line;\
		(cur).start.column = (cur).end.column = YYRHSLOC(rhs, 0).end.column;\
	}\
}


static void yyerror(YYLTYPE* yylloc, yyscan_t scanner, char const *token)
{
	(void) yylloc;
	(void) scanner;
	(void) token;
}
%}


%param {yyscan_t scanner}


%code requires {
#include <jkcc/lexer.h>
#include <jkcc/parser.h>
#include <jkcc/string.h>


#define YYLTYPE location_t


// handle reentrant flex-bison cyclic dependency
typedef void* yyscan_t;
}


%union{
	string_t             string;
	integer_constant_t   integer_constant;
	floating_constant_t  floating_constant;
	character_constant_t character_constant;
	string_literal_t     string_literal;
}


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

%token <string> IDENTIFIER

%token <integer_constant>   INTEGER_CONSTANT
%token <floating_constant>  FLOATING_CONSTANT
%token <character_constant> CHARACTER_CONSTANT

%token <string_literal> STRING_LITERAL

%token PUNCTUATOR_LBRACKET
%token PUNCTUATOR_RBRACKET
%token PUNCTUATOR_LPARENTHESIS
%token PUNCTUATOR_RPARENTHESIS
%token PUNCTUATOR_LBRACE
%token PUNCTUATOR_RBRACE
%token PUNCTUATOR_MEMBER_ACCESS
%token PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE
%token PUNCTUATOR_INCREMENT
%token PUNCTUATOR_DECREMENT
%token PUNCTUATOR_AMPERSAND
%token PUNCTUATOR_ASTERISK
%token PUNCTUATOR_PLUS
%token PUNCTUATOR_MINUS
%token PUNCTUATOR_UNARY_COMPLEMENT
%token PUNCTUATOR_LOGICAL_NOT
%token PUNCTUATOR_DIVISION
%token PUNCTUATOR_MODULO
%token PUNCTUATOR_LBITSHIFT
%token PUNCTUATOR_RBITSHIFT
%token PUNCTUATOR_LESS_THAN
%token PUNCTUATOR_GREATER_THAN
%token PUNCTUATOR_LESS_THAN_OR_EQUAL
%token PUNCTUATOR_GREATER_THAN_OR_EQUAL
%token PUNCTUATOR_EQUALITY
%token PUNCTUATOR_INEQUALITY
%token PUNCTUATOR_XOR
%token PUNCTUATOR_OR
%token PUNCTUATOR_LOGICAL_AND
%token PUNCTUATOR_LOGICAL_OR
%token PUNCTUATOR_CONDITIONAL_QUESTION
%token PUNCTUATOR_CONDITIONAL_COLON
%token PUNCTUATOR_SEMICOLON
%token PUNCTUATOR_VARIADIC
%token PUNCTUATOR_ASSIGNMENT
%token PUNCTUATOR_COMPOUND_MULTIPLICATION
%token PUNCTUATOR_COMPOUND_DIVISION
%token PUNCTUATOR_COMPOUND_MODULO
%token PUNCTUATOR_COMPOUND_ADDITION
%token PUNCTUATOR_COMPOUND_SUBTRACTION
%token PUNCTUATOR_COMPOUND_LBITSHIFT
%token PUNCTUATOR_COMPOUND_RBITSHIFT
%token PUNCTUATOR_COMPOUND_AND
%token PUNCTUATOR_COMPOUND_XOR
%token PUNCTUATOR_COMPOUND_OR
%token PUNCTUATOR_COMMA
%token PUNCTUATOR_PREPROCESSOR
%token PUNCTUATOR_PREPROCESSOR_PASTING


%destructor { string_free(&$$.string); } <string_literal>


%%


primary_expression:
  identifier
| constant
| string_literal
| PUNCTUATOR_LPARENTHESIS expression PUNCTUATOR_RPARENTHESIS
| generic_selection
;


identifier:
  %empty
;


constant:
  %empty
;


string_literal:
  %empty
;


expression:
  %empty
;


generic_selection:
  %empty
;
