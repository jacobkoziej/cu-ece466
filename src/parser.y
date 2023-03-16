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

#include <jkcc/ast.h>
#include <jkcc/lexer.h>
#include <jkcc/parser.h>
#include <jkcc/string.h>
#include <jkcc/trace.h>


#define TRACE(rule, match) TRACE_RULE(parser->trace, rule, match)

#define YYLLOC_DEFAULT(cur, rhs, n) {\
	if (n) {\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.offset;\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.line;\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.column;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.offset;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.line;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.column;\
	} else {\
		(cur).file = YYRHSLOC(rhs, 0).file;\
		(cur).start.offset = (cur).end.offset = YYRHSLOC(rhs, 0).end.offset;\
		(cur).start.line   = (cur).end.line   = YYRHSLOC(rhs, 0).end.line;\
		(cur).start.column = (cur).end.column = YYRHSLOC(rhs, 0).end.column;\
	}\
}


static void yyerror(
	YYLTYPE            *yylloc,
	yyscan_t            scanner,
	parser_t           *parser,
	translation_unit_t *translation_unit,
	char const         *token)
{
	(void) yylloc;
	(void) scanner;
	(void) parser;
	(void) translation_unit;
	(void) token;
}
%}


%param {yyscan_t scanner}
%parse-param {parser_t *parser}
%parse-param {translation_unit_t *translation_unit}


%code requires {
#include <jkcc/ast.h>
#include <jkcc/lexer.h>
#include <jkcc/parser.h>
#include <jkcc/string.h>


#define YYLTYPE location_t


// handle reentrant flex-bison cyclic dependency
typedef void* yyscan_t;
}


%union{
	identifier_t          identifier;
	integer_constant_t    integer_constant;
	floating_constant_t   floating_constant;
	character_constant_t  character_constant;
	string_literal_t      string_literal;
	ast_t                *ast;
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

%token <identifier> IDENTIFIER

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

%nterm <ast> identifier
%nterm <ast> integer_constant
%nterm <ast> floating_constant
%nterm <ast> character_constant
%nterm <ast> string_literal
%nterm <ast> unary_operator
%nterm <ast> assignment_operator
%nterm <ast> storage_class_specifier
%nterm <ast> type_qualifier


%destructor {
	string_free(&$$.IDENTIFIER);
	string_free(&$$.text);
} <identifier>
%destructor { AST_NODE_FREE($$); } <ast>


%%


primary_expression:
  identifier
| constant
| string_literal
| PUNCTUATOR_LPARENTHESIS expression PUNCTUATOR_RPARENTHESIS
| generic_selection
;


identifier: IDENTIFIER {
	TRACE("identifier", "IDENTIFIER");

	$identifier = ast_identifier_init(&$IDENTIFIER, &@IDENTIFIER);
	if (!$identifier) YYNOMEM;
}


constant:
  integer_constant
| floating_constant
// | enumeration_constant
| character_constant
;


integer_constant: INTEGER_CONSTANT {
	TRACE("integer_consant", "INTEGER_CONSTANT");

	$integer_constant = ast_integer_constant_init(
		&$INTEGER_CONSTANT,
		&@INTEGER_CONSTANT);
	if (!$integer_constant) YYNOMEM;
}


floating_constant: FLOATING_CONSTANT {
	TRACE("floating_constant", "FLOATING_CONSTANT");

	$floating_constant = ast_floating_constant_init(
		&$FLOATING_CONSTANT,
		&@FLOATING_CONSTANT);
	if (!$floating_constant) YYNOMEM;
}


/* TODO: add support for enum's
enumeration_constant: IDENTIFIER {
}
*/


character_constant: CHARACTER_CONSTANT {
	TRACE("character_constant", "CHARACTER_CONSTANT");

	$character_constant = ast_character_constant_init(
		&$CHARACTER_CONSTANT,
		&@CHARACTER_CONSTANT);
	if (!$character_constant) YYNOMEM;
}


string_literal: STRING_LITERAL {
	TRACE("string_literal", "STRING_LITERAL");

	$string_literal = ast_string_literal_init(
		&$STRING_LITERAL,
		&@STRING_LITERAL);
	if (!$string_literal) YYNOMEM;
}


expression:
  %empty
;


generic_selection:
  %empty
;


unary_operator:
  PUNCTUATOR_AMPERSAND {
	TRACE("unary_operator", "PUNCTUATOR_AMPERSAND");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_AMPERSAND,
		&@PUNCTUATOR_AMPERSAND);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_ASTERISK {
	TRACE("unary_operator", "PUNCTUATOR_ASTERISK");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_ASTERISK,
		&@PUNCTUATOR_ASTERISK);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_PLUS {
	TRACE("unary_operator", "PUNCTUATOR_PLUS");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_PLUS,
		&@PUNCTUATOR_PLUS);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_MINUS {
	TRACE("unary_operator", "PUNCTUATOR_MINUS");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_MINUS,
		&@PUNCTUATOR_MINUS);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_UNARY_COMPLEMENT {
	TRACE("unary_operator", "PUNCTUATOR_UNARY_COMPLEMENT");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_UNARY_COMPLEMENT,
		&@PUNCTUATOR_UNARY_COMPLEMENT);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_LOGICAL_NOT {
	TRACE("unary_operator", "PUNCTUATOR_LOGICAL_NOT");

	$unary_operator = ast_unary_operator_init(
		PUNCTUATOR_LOGICAL_NOT,
		&@PUNCTUATOR_LOGICAL_NOT);
	if (!$unary_operator) YYNOMEM;
}
;


assignment_operator:
  PUNCTUATOR_ASSIGNMENT {
	TRACE("assignment_operator", "PUNCTUATOR_ASSIGNMENT");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_ASSIGNMENT,
		&@PUNCTUATOR_ASSIGNMENT);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_MULTIPLICATION {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_MULTIPLICATION");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_MULTIPLICATION,
		&@PUNCTUATOR_COMPOUND_MULTIPLICATION);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_DIVISION {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_DIVISION");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_DIVISION,
		&@PUNCTUATOR_COMPOUND_DIVISION);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_MODULO {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_MODULO");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_MODULO,
		&@PUNCTUATOR_COMPOUND_MODULO);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_ADDITION {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_ADDITION");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_ADDITION,
		&@PUNCTUATOR_COMPOUND_ADDITION);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_SUBTRACTION {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_SUBTRACTION");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_SUBTRACTION,
		&@PUNCTUATOR_COMPOUND_SUBTRACTION);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_LBITSHIFT {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_LBITSHIFT");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_LBITSHIFT,
		&@PUNCTUATOR_COMPOUND_LBITSHIFT);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_RBITSHIFT {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_RBITSHIFT");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_RBITSHIFT,
		&@PUNCTUATOR_COMPOUND_RBITSHIFT);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_AND {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_AND");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_AND,
		&@PUNCTUATOR_COMPOUND_AND);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_XOR {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_XOR");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_XOR,
		&@PUNCTUATOR_COMPOUND_XOR);
	if (!$assignment_operator) YYNOMEM;
}
| PUNCTUATOR_COMPOUND_OR {
	TRACE("assignment_operator", "PUNCTUATOR_COMPOUND_OR");

	$assignment_operator = ast_assignment_operator_init(
		PUNCTUATOR_COMPOUND_OR,
		&@PUNCTUATOR_COMPOUND_OR);
	if (!$assignment_operator) YYNOMEM;
}
;


storage_class_specifier:
  KEYWORD_TYPEDEF {
	TRACE("storage_class_specifier", "KEYWORD_TYPEDEF");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD_TYPEDEF,
		&@KEYWORD_TYPEDEF);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_EXTERN {
	TRACE("storage_class_specifier", "KEYWORD_EXTERN");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD_EXTERN,
		&@KEYWORD_EXTERN);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_STATIC {
	TRACE("storage_class_specifier", "KEYWORD_STATIC");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD_STATIC,
		&@KEYWORD_STATIC);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD__THREAD_LOCAL {
	TRACE("storage_class_specifier", "KEYWORD__THREAD_LOCAL");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD__THREAD_LOCAL,
		&@KEYWORD__THREAD_LOCAL);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_AUTO {
	TRACE("storage_class_specifier", "KEYWORD_AUTO");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD_AUTO,
		&@KEYWORD_AUTO);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_REGISTER {
	TRACE("storage_class_specifier", "KEYWORD_REGISTER");

	$storage_class_specifier = ast_storage_class_specifier_init(
		KEYWORD_REGISTER,
		&@KEYWORD_REGISTER);
	if (!$storage_class_specifier) YYNOMEM;
}
;


type_qualifier:
  KEYWORD_CONST {
	TRACE("type_qualifier", "KEYWORD_CONST");

	$type_qualifier = ast_type_qualifier_init(
		KEYWORD_CONST,
		&@KEYWORD_CONST);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD_RESTRICT {
	TRACE("type_qualifier", "KEYWORD_RESTRICT");

	$type_qualifier = ast_type_qualifier_init(
		KEYWORD_RESTRICT,
		&@KEYWORD_RESTRICT);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD_VOLATILE {
	TRACE("type_qualifier", "KEYWORD_VOLATILE");

	$type_qualifier = ast_type_qualifier_init(
		KEYWORD_VOLATILE,
		&@KEYWORD_VOLATILE);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD__ATOMIC {
	TRACE("type_qualifier", "KEYWORD__ATOMIC");

	$type_qualifier = ast_type_qualifier_init(
		KEYWORD__ATOMIC,
		&@KEYWORD__ATOMIC);
	if (!$type_qualifier) YYNOMEM;
}
;
