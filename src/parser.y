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

#include <stdbool.h>

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

%nterm <ast> identifier
%nterm <ast> constant
%nterm <ast> integer_constant
%nterm <ast> floating_constant
%nterm <ast> character_constant
%nterm <ast> string_literal
%nterm <ast> primary_expression
%nterm <ast> postfix_expression
%nterm <ast> unary_expression
%nterm <ast> unary_operator
%nterm <ast> cast_expression
%nterm <ast> multiplicative_expression
%nterm <ast> additive_expression
%nterm <ast> shift_expression
%nterm <ast> relational_expression
%nterm <ast> equality_expression
%nterm <ast> and_expression
%nterm <ast> exclusive_or_expression
%nterm <ast> inclusive_or_expression
%nterm <ast> logical_and_expression
%nterm <ast> logical_or_expression
%nterm <ast> conditional_expression
%nterm <ast> assignment_expression
%nterm <ast> assignment_operator
%nterm <ast> expression
%nterm <ast> constant_expression
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


// 6.4.4
constant:
  integer_constant {
	TRACE("constant", "integer-constant");
	$constant = $integer_constant;
}
| floating_constant {
	TRACE("constant", "floating-constant");
	$constant = $floating_constant;
}
/*
| enumeration_constant {
	TRACE("constant", "enumeration-constant");
	$constant = $enumeration_constant;
}
*/
| character_constant {
	TRACE("constant", "character-constant");
	$constant = $character_constant;
}
;


// 6.4.2.1
identifier: IDENTIFIER {
	TRACE("identifier", "IDENTIFIER");

	$identifier = ast_identifier_init(&$IDENTIFIER, &@IDENTIFIER);
	if (!$identifier) YYNOMEM;
}


// 6.4.4.1
integer_constant: INTEGER_CONSTANT {
	TRACE("integer_consant", "INTEGER_CONSTANT");

	$integer_constant = ast_integer_constant_init(
		&$INTEGER_CONSTANT,
		&@INTEGER_CONSTANT);
	if (!$integer_constant) YYNOMEM;
}


// 6.4.4.2
floating_constant: FLOATING_CONSTANT {
	TRACE("floating_constant", "FLOATING_CONSTANT");

	$floating_constant = ast_floating_constant_init(
		&$FLOATING_CONSTANT,
		&@FLOATING_CONSTANT);
	if (!$floating_constant) YYNOMEM;
}


/* TODO: add support for enum's
// 6.4.4.3
enumeration_constant: IDENTIFIER {
}
*/


// 6.4.4.4
character_constant: CHARACTER_CONSTANT {
	TRACE("character_constant", "CHARACTER_CONSTANT");

	$character_constant = ast_character_constant_init(
		&$CHARACTER_CONSTANT,
		&@CHARACTER_CONSTANT);
	if (!$character_constant) YYNOMEM;
}


// 6.4.5
string_literal: STRING_LITERAL {
	TRACE("string_literal", "STRING_LITERAL");

	$string_literal = ast_string_literal_init(
		&$STRING_LITERAL,
		&@STRING_LITERAL);
	if (!$string_literal) YYNOMEM;
}


// 6.5.1
primary_expression:
  identifier {
	TRACE("primary-expression", "identifier");
	$primary_expression = $identifier;
}
| constant {
	TRACE("primary-expression", "constant");
	$primary_expression = $constant;
}
| string_literal {
	TRACE("primary-expression", "string-literal");
	$primary_expression = $string_literal;
}
| PUNCTUATOR_LPARENTHESIS expression PUNCTUATOR_RPARENTHESIS {
	TRACE("primary-expression", "( expression )");
	$primary_expression = $expression;
}
/*
| generic_selection {
	TRACE("primary-expression", "generic-selection");
	$primary_expression = $generic_selection;
}
*/
;


// 6.5.1.1
generic_selection:
  %empty
;


// 6.5.2
postfix_expression:
  primary_expression {
	TRACE("postfix-expression", "primary-expression");
	$postfix_expression = $primary_expression;
}
// | postfix_expression PUNCTUATOR_LBRACKET expression PUNCTUATOR_RBRACKET
| postfix_expression[child] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix-expression", "postfix-expression ( )");

	$$ = ast_postfix_expression_init(
		$child,
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
	TRACE("postfix-expression", "postfix-expression . identifier");

	$$ = ast_postfix_expression_init(
		$child,
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
	TRACE("postfix-expression", "postfix-expression -> identifier");

	$$ = ast_postfix_expression_init(
		$child,
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
	TRACE("postfix-expression", "postfix-expression ++");

	$$ = ast_postfix_expression_init(
		$child,
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
	TRACE("postfix-expression", "postfix-expression --");

	$$ = ast_postfix_expression_init(
		$child,
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


// 6.5.3
unary_expression:
  postfix_expression {
	TRACE("unary-expression", "postfix-expression");
	$unary_expression = $postfix_expression;
}
| PUNCTUATOR_INCREMENT unary_expression[child] {
	TRACE("unary-expression", "++ unary-expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_INCREMENT,
		&@PUNCTUATOR_INCREMENT,
		&@child);
	if (!$$) YYNOMEM;
}
| PUNCTUATOR_DECREMENT unary_expression[child] {
	TRACE("unary-expression", "-- unary-expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_DECREMENT,
		&@PUNCTUATOR_DECREMENT,
		&@child);
	if (!$$) YYNOMEM;
}
| unary_operator cast_expression {
	TRACE("unary-expression", "unary-operator cast-expression");

	$$ = ast_unary_expression_init(
		NULL,
		$unary_operator,
		$cast_expression,
		NULL,
		0,
		&@unary_operator,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
| KEYWORD_SIZEOF unary_expression[child] {
	TRACE("unary-expression", "sizeof unary-expression");

	$$ = ast_unary_expression_init(
		$child,
		NULL,
		NULL,
		NULL,
		UNARY_EXPRESSION_SIZEOF,
		&@KEYWORD_SIZEOF,
		&@child);
	if (!$$) YYNOMEM;
}
| KEYWORD_SIZEOF PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS {
	TRACE("unary-expression", "sizeof ( type-name )");

	$unary_expression = ast_unary_expression_init(
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
	TRACE("unary-expression", "_Alignof ( type-name )");

	$unary_expression = ast_unary_expression_init(
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


// 6.5.3
unary_operator:
  PUNCTUATOR_AMPERSAND {
	TRACE("unary-operator", "&");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_AMPERSAND,
		&@PUNCTUATOR_AMPERSAND);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_ASTERISK {
	TRACE("unary-operator", "*");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_ASTERISK,
		&@PUNCTUATOR_ASTERISK);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_PLUS {
	TRACE("unary-operator", "+");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_PLUS,
		&@PUNCTUATOR_PLUS);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_MINUS {
	TRACE("unary-operator", "-");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_MINUS,
		&@PUNCTUATOR_MINUS);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_UNARY_COMPLEMENT {
	TRACE("unary-operator", "~");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_UNARY_COMPLEMENT,
		&@PUNCTUATOR_UNARY_COMPLEMENT);
	if (!$unary_operator) YYNOMEM;
}
| PUNCTUATOR_LOGICAL_NOT {
	TRACE("unary-operator", "!");

	$unary_operator = ast_unary_operator_init(
		UNARY_OPERATOR_LOGICAL_NOT,
		&@PUNCTUATOR_LOGICAL_NOT);
	if (!$unary_operator) YYNOMEM;
}
;


// 6.5.4
cast_expression:
  unary_expression {
	TRACE("cast-expression", "unary-expression");
	$cast_expression = $unary_expression;
}
| PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS cast_expression[child] {
	TRACE("cast-expression", "( type-name ) cast-expression");

	$$ = ast_cast_expression_init(
		$child,
		$type_name,
		&@PUNCTUATOR_LPARENTHESIS,
		&@child);
	if (!$$) YYNOMEM;
}
;


// 6.5.4
multiplicative_expression:
  cast_expression {
	TRACE("multiplicative-expression", "cast-expression");
	$multiplicative_expression = $cast_expression;
}
| multiplicative_expression[child] PUNCTUATOR_ASTERISK cast_expression {
	TRACE("multiplicative-expression", "multiplicative-expression * cast-expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_MULTIPLICATION,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[child] PUNCTUATOR_DIVISION cast_expression {
	TRACE("multiplicative-expression", "multiplicative-expression / cast-expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_DIVISION,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[child] PUNCTUATOR_MODULO cast_expression {
	TRACE("multiplicative-expression", "multiplicative-expression % cast-expression");

	$$ = ast_multiplicative_expression_init(
		$child,
		$cast_expression,
		MULTIPLICATIVE_EXPRESSION_MODULO,
		&@child,
		&@cast_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.6
additive_expression:
  multiplicative_expression {
	TRACE("additive-expression", "multiplicative-expression");
	$additive_expression = $multiplicative_expression;
}
| additive_expression[child] PUNCTUATOR_PLUS multiplicative_expression {
	TRACE("additive-expression", "additive-expression + multiplicative-expression");

	$$ = ast_additive_expression_init(
		$child,
		$multiplicative_expression,
		ADDITIVE_EXPRESSION_ADDITION,
		&@child,
		&@multiplicative_expression);
	if (!$$) YYNOMEM;
}
| additive_expression[child] PUNCTUATOR_MINUS multiplicative_expression {
	TRACE("additive-expression", "additive-expression - multiplicative-expression");

	$$ = ast_additive_expression_init(
		$child,
		$multiplicative_expression,
		ADDITIVE_EXPRESSION_SUBTRACTION,
		&@child,
		&@multiplicative_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.7
shift_expression:
  additive_expression {
	TRACE("shift-expression", "additive-expression");
	$shift_expression = $additive_expression;
}
| shift_expression[child] PUNCTUATOR_LBITSHIFT additive_expression {
	TRACE("shift-expression", "shift-expression << additive-expression");

	$$ = ast_shift_expression_init(
		$child,
		$additive_expression,
		SHIFT_EXPRESSION_LBITSHIFT,
		&@child,
		&@additive_expression);
	if (!$$) YYNOMEM;
}
| shift_expression[child] PUNCTUATOR_RBITSHIFT additive_expression {
	TRACE("shift-expression", "shift-expression >> additive-expression");

	$$ = ast_shift_expression_init(
		$child,
		$additive_expression,
		SHIFT_EXPRESSION_RBITSHIFT,
		&@child,
		&@additive_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.8
relational_expression:
  shift_expression {
	TRACE("relational-expression", "shift-expression");
	$relational_expression = $shift_expression;
}
| relational_expression[child] PUNCTUATOR_LESS_THAN shift_expression {
	TRACE("relational-expression", "relational-expression < shift-expression");

	$$ = ast_relational_expression_init(
		$child,
		$shift_expression,
		RELATIONAL_EXPRESSION_LESS_THAN,
		&@child,
		&@shift_expression);
	if (!$$) YYNOMEM;
}
| relational_expression[child] PUNCTUATOR_GREATER_THAN shift_expression {
	TRACE("relational-expression", "relational-expression > shift-expression");

	$$ = ast_relational_expression_init(
		$child,
		$shift_expression,
		RELATIONAL_EXPRESSION_GREATER_THAN,
		&@child,
		&@shift_expression);
	if (!$$) YYNOMEM;
}
| relational_expression[child] PUNCTUATOR_LESS_THAN_OR_EQUAL shift_expression {
	TRACE("relational-expression", "relational-expression <= shift-expression");

	$$ = ast_relational_expression_init(
		$child,
		$shift_expression,
		RELATIONAL_EXPRESSION_LESS_THAN_OR_EQUAL,
		&@child,
		&@shift_expression);
	if (!$$) YYNOMEM;
}
| relational_expression[child] PUNCTUATOR_GREATER_THAN_OR_EQUAL shift_expression {
	TRACE("relational-expression", "relational-expression >= shift-expression");

	$$ = ast_relational_expression_init(
		$child,
		$shift_expression,
		RELATIONAL_EXPRESSION_GREATER_THAN_OR_EQUAL,
		&@child,
		&@shift_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.9
equality_expression:
  relational_expression {
	TRACE("equality-expression", "relational-expression");
	$equality_expression = $relational_expression;
}
| equality_expression[child] PUNCTUATOR_EQUALITY relational_expression {
	TRACE("equality-expression", "equality-expression == relational-expression");

	$$ = ast_equality_expression_init(
		$child,
		$relational_expression,
		EQUALITY_EXPRESSION_EQUALITY,
		&@child,
		&@relational_expression);
	if (!$$) YYNOMEM;
}
| equality_expression[child] PUNCTUATOR_INEQUALITY relational_expression {
	TRACE("equality-expression", "equality-expression != relational-expression");

	$$ = ast_equality_expression_init(
		$child,
		$relational_expression,
		EQUALITY_EXPRESSION_INEQUALITY,
		&@child,
		&@relational_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.10
and_expression:
  equality_expression {
	TRACE("AND-expression", "equality-expression");
	$and_expression = $equality_expression;
}
| and_expression[child] PUNCTUATOR_AMPERSAND equality_expression {
	TRACE("AND-expression", "AND-expression & equality-expression");

	$$ = ast_and_expression_init(
		$child,
		$equality_expression,
		&@child,
		&@equality_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.11
exclusive_or_expression:
  and_expression {
	TRACE("exclusive-OR-expression", "AND-expression");
	$exclusive_or_expression = $and_expression;
}
| exclusive_or_expression[child] PUNCTUATOR_XOR and_expression {
	TRACE("exclusive-OR-expression", "exclusive-OR-expression ^ AND-expression");

	$$ = ast_exclusive_or_expression_init(
		$child,
		$and_expression,
		&@child,
		&@and_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.12
inclusive_or_expression:
  exclusive_or_expression {
	TRACE("inclusive-OR-expression", "exclusive-OR-expression");
	$inclusive_or_expression = $exclusive_or_expression;
}
| inclusive_or_expression[child] PUNCTUATOR_OR exclusive_or_expression {
	TRACE("inclusive-OR-expression", "inclusive-OR-expression | exclusive-OR-expression");

	$$ = ast_inclusive_or_expression_init(
		$child,
		$exclusive_or_expression,
		&@child,
		&@exclusive_or_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.13
logical_and_expression:
  inclusive_or_expression {
	TRACE("logical-AND-expression", "inclusive-OR-expression");
	$logical_and_expression = $inclusive_or_expression;
}
| logical_and_expression[child] PUNCTUATOR_LOGICAL_AND inclusive_or_expression {
	TRACE("logical-AND-expression", "logical-AND-expression && inclusive-OR-expression");

	$$ = ast_logical_and_expression_init(
		$child,
		$inclusive_or_expression,
		&@child,
		&@inclusive_or_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.14
logical_or_expression:
  logical_and_expression {
	TRACE("logical-OR-expression", "logical-AND-expression");
	$logical_or_expression = $logical_and_expression;
}
| logical_or_expression[child] PUNCTUATOR_LOGICAL_OR logical_and_expression {
	TRACE("logical-OR-expression", "logical-OR-expression || logical-AND-expression");

	$$ = ast_logical_or_expression_init(
		$child,
		$logical_and_expression,
		&@child,
		&@logical_and_expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.15
conditional_expression:
  logical_or_expression {
	TRACE("conditional-expression", "logical-OR-expression");
	$conditional_expression = $logical_or_expression;
}
| logical_or_expression PUNCTUATOR_CONDITIONAL_QUESTION expression PUNCTUATOR_CONDITIONAL_COLON conditional_expression[child] {
	TRACE("conditional-expression", "logical-OR-expression ? expression : conditional-expression");

	$$ = ast_conditional_expression_init(
		$logical_or_expression,
		$expression,
		$child,
		&@logical_or_expression,
		&@child);
	if (!$$) YYNOMEM;
}
;


// 6.5.16
assignment_expression:
  conditional_expression {
	TRACE("assignment-expression", "conditional-expression");
	$assignment_expression = $conditional_expression;
}
| unary_expression assignment_operator assignment_expression[child] {
	TRACE("assignment-expression", "unary-expression assignment-operator assignment-expression");

	$$ = ast_assignment_expression_init(
		$unary_expression,
		$assignment_operator,
		$child,
		&@unary_expression,
		&@child);
	if (!$$) YYNOMEM;
}
;


// 6.5.16
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


// 6.5.17
expression:
  assignment_expression {
	TRACE("expression", "assignment_expression");

	$expression = ast_expression_init(
		$assignment_expression,
		&@assignment_expression);
	if (!$expression) YYNOMEM;
}
| expression[child] PUNCTUATOR_COMMA assignment_expression {
	TRACE("expression", "expression PUNCTUATOR_COMMA assignment_expression");

	$$ = ast_expression_append(
		$child,
		$assignment_expression,
		&@assignment_expression);
	if (!$$) YYNOMEM;
}
;


// 6.6
constant_expression: conditional_expression {
	TRACE("constant-expression", "conditional-expression");
	$constant_expression = $conditional_expression;
}


// 6.7.1
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


// 6.7.2
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


// 6.7.2.1
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


// 6.7.3
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


// 6.7.6
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


// 6.7.6
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


// 6.7.7
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
