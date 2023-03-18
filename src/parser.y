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


#define ERROR(ast) {                              \
	if (!ast) {                               \
		if (parser->error) {              \
			yyerror(                  \
				&yylloc,          \
				scanner,          \
				parser,           \
				translation_unit, \
				parser->error);   \
			YYERROR;                  \
		} else {                          \
			YYNOMEM;                  \
		}                                 \
	}                                         \
}

#define TRACE(rule, match) TRACE_RULE(parser->trace, rule, match)

#define YYLLOC_DEFAULT(cur, rhs, n) {\
	if (n) {\
		(cur).file = YYRHSLOC(rhs, 1).file;\
		(cur).start.offset = YYRHSLOC(rhs, 1).start.offset;\
		(cur).start.line   = YYRHSLOC(rhs, 1).start.line;\
		(cur).start.column = YYRHSLOC(rhs, 1).start.column;\
		(cur).end.offset   = YYRHSLOC(rhs, n).end.offset;\
		(cur).end.line     = YYRHSLOC(rhs, n).end.line;\
		(cur).end.column   = YYRHSLOC(rhs, n).end.column;\
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
	char const         *error)
{
	(void) yylloc;
	(void) scanner;
	(void) parser;
	(void) translation_unit;

	fprintf(stderr, "error: %s\n", error);
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

%nterm <ast> primary_expression
%nterm <ast> identifier
%nterm <ast> integer_constant
%nterm <ast> floating_constant
%nterm <ast> character_constant
%nterm <ast> constant
%nterm <ast> string_literal
%nterm <ast> postfix_expression
%nterm <ast> unary_expression
%nterm <ast> cast_expression
%nterm <ast> multiplicative_expression
%nterm <ast> additive_expression
%nterm <ast> unary_operator
%nterm <ast> assignment_operator
%nterm <ast> storage_class_specifier
%nterm <ast> type_specifier
%nterm <ast> specifier_qualifier_list
%nterm <ast> type_qualifier
%nterm <ast> pointer
%nterm <ast> type_qualifier_list
%nterm <ast> type_name


%destructor {
	string_free(&$$.IDENTIFIER);
	string_free(&$$.text);
} <identifier>
%destructor { AST_NODE_FREE($$); } <ast>


%%


primary_expression:
  identifier {
	TRACE("primary_expression", "identifier");

	$primary_expression = ast_primary_expression_init(
		$identifier,
		&@identifier);
	if (!$primary_expression) YYNOMEM;
}
| constant {
	TRACE("primary_expression", "constant");

	$primary_expression = ast_primary_expression_init(
		$constant,
		&@constant);
	if (!$primary_expression) YYNOMEM;
}
| string_literal {
	TRACE("primary_expression", "string_literal");

	$primary_expression = ast_primary_expression_init(
		$string_literal,
		&@string_literal);
	if (!$primary_expression) YYNOMEM;
}
/*
| PUNCTUATOR_LPARENTHESIS expression PUNCTUATOR_RPARENTHESIS {
	TRACE("primary_expression", "PUNCTUATOR_LPARENTHESIS expression PUNCTUATOR_RPARENTHESIS");

	$primary_expression = ast_primary_expression_init(
		$expression,
		&@expression);
	if (!$primary_expression) YYNOMEM;
}
| generic_selection {
	TRACE("primary_expression", "generic_selection");

	$primary_expression = ast_primary_expression_init(
		$generic_selection,
		&@generic_selection);
	if (!$primary_expression) YYNOMEM;
}
*/
;


identifier: IDENTIFIER {
	TRACE("identifier", "IDENTIFIER");

	$identifier = ast_identifier_init(&$IDENTIFIER, &@IDENTIFIER);
	if (!$identifier) YYNOMEM;
}


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


constant:
  integer_constant {
	TRACE("constant", "integer_constant");

	$constant = ast_constant_init(
		$integer_constant,
		&@integer_constant);
	if (!$constant) YYNOMEM;
}
| floating_constant {
	TRACE("constant", "floating_constant");

	$constant = ast_constant_init(
		$floating_constant,
		&@floating_constant);
	if (!$constant) YYNOMEM;
}
/*
| enumeration_constant {
	TRACE("constant", "enumeration_constant");

	$constant = ast_constant_init(
		$enumeration_constant,
		&@enumeration_constant);
	if (!$constant) YYNOMEM;
}
*/
| character_constant {
	TRACE("constant", "character_constant");

	$constant = ast_constant_init(
		$character_constant,
		&@character_constant);
	if (!$constant) YYNOMEM;
}
;


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


postfix_expression:
  primary_expression {
	TRACE("postfix_expression", "primary_expression");

	$postfix_expression = ast_postfix_expression_init(
		NULL,
		$primary_expression,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0,
		&@primary_expression,
		NULL);
	if (!$postfix_expression) YYNOMEM;
}
// | postfix_expression PUNCTUATOR_LBRACKET expression PUNCTUATOR_RBRACKET
| postfix_expression[child] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix_expression", "postfix_expression PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS");

	$$ = ast_postfix_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		POSTFIX_EXPRESSION_PARENTHESIS,
		&@child,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$$) YYNOMEM;
}
// | postfix_expression PUNCTUATOR_LPARENTHESIS argument_expression_list PUNCTUATOR_RPARENTHESIS
| postfix_expression[child] PUNCTUATOR_MEMBER_ACCESS identifier {
	TRACE("postfix_expression", "postfix_expression PUNCTUATOR_MEMBER_ACCESS identifier");

	$$ = ast_postfix_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		$identifier,
		NULL,
		NULL,
		0,
		&@child,
		&@identifier);
	if (!$$) YYNOMEM;
}
| postfix_expression[child] PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE identifier {
	TRACE("postfix_expression", "postfix_expression PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE identifier");

	$$ = ast_postfix_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		$identifier,
		NULL,
		NULL,
		POSTFIX_EXPRESSION_DEREFERENCE,
		&@child,
		&@identifier);
	if (!$$) YYNOMEM;
}
| postfix_expression[child] PUNCTUATOR_INCREMENT[increment] {
	TRACE("postfix_expression", "postfix_expression PUNCTUATOR_INCREMENT");

	$$ = ast_postfix_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		POSTFIX_EXPRESSION_INCREMENT,
		&@child,
		&@increment);
	if (!$$) YYNOMEM;
}
| postfix_expression[child] PUNCTUATOR_DECREMENT[decrement] {
	TRACE("postfix_expression", "postfix_expression PUNCTUATOR_DECREMENT");

	$$ = ast_postfix_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		POSTFIX_EXPRESSION_DECREMENT,
		&@child,
		&@decrement);
	if (!$$) YYNOMEM;
}
// | PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS PUNCTUATOR_LBRACKET initializer_list PUNCTUATOR_RBRACKET
// | PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS PUNCTUATOR_LBRACKET initializer_list PUNCTUATOR_COMMA PUNCTUATOR_RBRACKET
;


unary_expression:
  postfix_expression {
	TRACE("unary_expression", "postfix_expression");

	$unary_expression = ast_unary_expression_init(
		NULL,
		$postfix_expression,
		NULL,
		NULL,
		NULL,
		0,
		&@postfix_expression,
		NULL);
	if (!$unary_expression) YYNOMEM;
}
| PUNCTUATOR_INCREMENT unary_expression[child] {
	TRACE("unary_expression", "PUNCTUATOR_INCREMENT unary_expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_INCREMENT,
		&@PUNCTUATOR_INCREMENT,
		&@child);
	if (!$$) YYNOMEM;
}
| PUNCTUATOR_DECREMENT unary_expression[child] {
	TRACE("unary_expression", "PUNCTUATOR_DECREMENT unary_expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_DECREMENT,
		&@PUNCTUATOR_DECREMENT,
		&@child);
	if (!$$) YYNOMEM;
}
// | unary_operator cast_expression
| KEYWORD_SIZEOF unary_expression[child] {
	TRACE("unary_expression", "KEYWORD_SIZEOF unary_expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_SIZEOF,
		&@KEYWORD_SIZEOF,
		&@child);
	if (!$$) YYNOMEM;
}
| KEYWORD_SIZEOF PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS {
	TRACE("unary_expression", "KEYWORD_SIZEOF PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS");

	$unary_expression = ast_unary_expression_init(
		NULL,
		NULL,
		NULL,
		NULL,
		$type_name,
		UNARY_EXPRESSION_SIZEOF,
		&@KEYWORD_SIZEOF,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$unary_expression) YYNOMEM;
}
| KEYWORD__ALIGNOF PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS {
	TRACE("unary_expression", "KEYWORD__ALIGNOF PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS");

	$unary_expression = ast_unary_expression_init(
		NULL,
		NULL,
		NULL,
		NULL,
		$type_name,
		UNARY_EXPRESSION__ALIGNOF,
		&@KEYWORD__ALIGNOF,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$unary_expression) YYNOMEM;
}
;


multiplicative_expression:
  cast_expression {
	TRACE("multiplicative_expression", "cast_expression");

	$multiplicative_expression = ast_multiplicative_expression_init(
		NULL,
		$cast_expression,
		0,
		&@cast_expression,
		NULL);
	if (!$multiplicative_expression) YYNOMEM;
}
| multiplicative_expression[child] PUNCTUATOR_ASTERISK cast_expression {
	TRACE("multiplicative_expression", "multiplicative_expression PUNCTUATOR_ASTERISK cast_expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_MULTIPLICATION,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[child] PUNCTUATOR_DIVISION cast_expression {
	TRACE("multiplicative_expression", "multiplicative_expression PUNCTUATOR_DIVISION cast_expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_DIVISION,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[child] PUNCTUATOR_MODULO cast_expression {
	TRACE("multiplicative_expression", "multiplicative_expression PUNCTUATOR_MODULO cast_expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_MODULO,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
;


additive_expression:
  multiplicative_expression {
	TRACE("additive_expression", "multiplicative_expression");

	$additive_expression = ast_additive_expression_init(
		NULL,
		$multiplicative_expression,
		0,
		&@multiplicative_expression,
		NULL);
	if (!$additive_expression) YYNOMEM;
}
| additive_expression[child] PUNCTUATOR_PLUS multiplicative_expression {
	TRACE("additive_expression", "additive_expression PUNCTUATOR_PLUS multiplicative_expression");

	$$ = ast_additive_expression_init(
		$child,
		$multiplicative_expression,
		ADDITIVE_EXPRESSION_ADDITION,
		&@child,
		&@multiplicative_expression);
	if (!$$) YYNOMEM;
}
| additive_expression[child] PUNCTUATOR_MINUS multiplicative_expression {
	TRACE("additive_expression", "additive_expression PUNCTUATOR_MINUS multiplicative_expression");

	$$ = ast_additive_expression_init(
		$child,
		$multiplicative_expression,
		ADDITIVE_EXPRESSION_SUBTRACTION,
		&@child,
		&@multiplicative_expression);
	if (!$$) YYNOMEM;
}
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


cast_expression:
  unary_expression {
	TRACE("cast_expression", "unary_expression");

	$cast_expression = ast_cast_expression_init(
		NULL,
		$unary_expression,
		NULL,
		&@unary_expression,
		NULL);
	if (!$cast_expression) YYNOMEM;
}
| PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS cast_expression[child] {
	TRACE("cast_expression", "PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS cast_expression");

	$$ = ast_cast_expression_init(
		$child,
		NULL,
		$type_name,
		&@PUNCTUATOR_LPARENTHESIS,
		&@child);
	if (!$$) YYNOMEM;
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


type_specifier:
  KEYWORD_VOID {
	TRACE("type_specifier", "KEYWORD_VOID");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_VOID,
		NULL,
		&@KEYWORD_VOID);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_CHAR {
	TRACE("type_specifier", "KEYWORD_CHAR");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_CHAR,
		NULL,
		&@KEYWORD_CHAR);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_SHORT {
	TRACE("type_specifier", "KEYWORD_SHORT");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_SHORT,
		NULL,
		&@KEYWORD_SHORT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_INT {
	TRACE("type_specifier", "KEYWORD_INT");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_INT,
		NULL,
		&@KEYWORD_INT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_LONG {
	TRACE("type_specifier", "KEYWORD_LONG");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_LONG,
		NULL,
		&@KEYWORD_LONG);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_FLOAT {
	TRACE("type_specifier", "KEYWORD_FLOAT");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_FLOAT,
		NULL,
		&@KEYWORD_FLOAT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_DOUBLE {
	TRACE("type_specifier", "KEYWORD_DOUBLE");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_DOUBLE,
		NULL,
		&@KEYWORD_DOUBLE);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_SIGNED {
	TRACE("type_specifier", "KEYWORD_SIGNED");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_SIGNED,
		NULL,
		&@KEYWORD_SIGNED);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_UNSIGNED {
	TRACE("type_specifier", "KEYWORD_UNSIGNED");

	$type_specifier = ast_type_specifier_init(
		KEYWORD_UNSIGNED,
		NULL,
		&@KEYWORD_UNSIGNED);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD__BOOL {
	TRACE("type_specifier", "KEYWORD__BOOL");

	$type_specifier = ast_type_specifier_init(
		KEYWORD__BOOL,
		NULL,
		&@KEYWORD__BOOL);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD__COMPLEX {
	TRACE("type_specifier", "KEYWORD__COMPLEX");

	$type_specifier = ast_type_specifier_init(
		KEYWORD__COMPLEX,
		NULL,
		&@KEYWORD__COMPLEX);
	if (!$type_specifier) YYNOMEM;
}
// | atomic_type_specifier
// | struct_or_union_specifier
// | enum_specifier
// | typedef_name
;


specifier_qualifier_list:
  type_specifier {
	TRACE("specifier_qualifier_list", "type_specifier");

	$specifier_qualifier_list = ast_specifier_qualifier_list_init(
		$type_specifier,
		NULL,
		&@type_specifier);
	if (!$specifier_qualifier_list) YYNOMEM;
}
| type_specifier specifier_qualifier_list[list] {
	TRACE("specifier_qualifier_list", "type_specifier specifier_qualifier_list");

	$$ = ast_specifier_qualifier_list_append(
		$list,
		$type_specifier,
		NULL,
		&@type_specifier,
		&parser->error);
	ERROR($$);
}
| type_qualifier {
	TRACE("specifier_qualifier_list", "type_qualifier");

	$specifier_qualifier_list = ast_specifier_qualifier_list_init(
		NULL,
		$type_qualifier,
		&@type_qualifier);
	if (!$$) YYNOMEM;
}
| type_qualifier specifier_qualifier_list[list] {
	TRACE("specifier_qualifier_list", "type_qualifier specifier_qualifier_list");

	$$ = ast_specifier_qualifier_list_append(
		$list,
		NULL,
		$type_qualifier,
		&@type_qualifier,
		&parser->error);
	ERROR($$);
}


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


pointer:
  PUNCTUATOR_ASTERISK {
	TRACE("pointer", "PUNCTUATOR_ASTERISK");

	$pointer = ast_pointer_init(NULL, NULL, &@PUNCTUATOR_ASTERISK);
	if (!$pointer) YYNOMEM;
}
| PUNCTUATOR_ASTERISK type_qualifier_list {
	TRACE("pointer", "PUNCTUATOR_ASTERISK type_qualifier_list");

	$pointer = ast_pointer_init(
		$type_qualifier_list,
		NULL,
		&@PUNCTUATOR_ASTERISK);
	if (!$pointer) YYNOMEM;
}
| PUNCTUATOR_ASTERISK pointer[child] {
	TRACE("pointer", "PUNCTUATOR_ASTERISK pointer");

	$$ = ast_pointer_init(NULL, $child, &@PUNCTUATOR_ASTERISK);
	if (!$$) YYNOMEM;
}
| PUNCTUATOR_ASTERISK type_qualifier_list pointer[child] {
	TRACE("pointer", "PUNCTUATOR_ASTERISK type_qualifier_list pointer");

	$$ = ast_pointer_init(
		$type_qualifier_list,
		$child,
		&@PUNCTUATOR_ASTERISK);
	if (!$$) YYNOMEM;
}
;


type_qualifier_list:
  type_qualifier {
	TRACE("type_qualifier_list", "type_qualifier");

	$type_qualifier_list = ast_type_qualifier_list_init(
		$type_qualifier,
		&@type_qualifier);
	if (!$type_qualifier_list) YYNOMEM;
}
| type_qualifier_list[list] type_qualifier {
	TRACE("type_qualifier_list", "type_qualifier_list type_qualifier");

	$$ = ast_type_qualifier_list_append(
		$list,
		$type_qualifier,
		&@type_qualifier);
	if (!$$) YYNOMEM;
}
;


type_name:
  specifier_qualifier_list {
	TRACE("type_name", "specifier_qualifier_list");

	$type_name = ast_type_name_init(
		$specifier_qualifier_list,
		NULL,
		&@specifier_qualifier_list,
		NULL);
	if (!$type_name) YYNOMEM;
}
/*
| specifier_qualifier_list abstract_declarator {
	TRACE("type_name", "specifier_qualifier_list abstract_declarator");

	$type_name = ast_type_name_init(
		$specifier_qualifier_list,
		$abstract_declarator,
		&@specifier_qualifier_list,
		&@abstract_declarator);
	if (!$type_name) YYNOMEM;
}
*/
;
