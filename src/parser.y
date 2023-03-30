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

#define YYLLOC_DEFAULT(cur, rhs, n) {                                                \
	if (n) {                                                                     \
		(cur).file = YYRHSLOC(rhs, 1).file;                                  \
		(cur).start.offset = YYRHSLOC(rhs, 1).start.offset;                  \
		(cur).start.line   = YYRHSLOC(rhs, 1).start.line;                    \
		(cur).start.column = YYRHSLOC(rhs, 1).start.column;                  \
		(cur).end.offset   = YYRHSLOC(rhs, n).end.offset;                    \
		(cur).end.line     = YYRHSLOC(rhs, n).end.line;                      \
		(cur).end.column   = YYRHSLOC(rhs, n).end.column;                    \
	} else {                                                                     \
		(cur).file = YYRHSLOC(rhs, 0).file;                                  \
		(cur).start.offset = (cur).end.offset = YYRHSLOC(rhs, 0).end.offset; \
		(cur).start.line   = (cur).end.line   = YYRHSLOC(rhs, 0).end.line;   \
		(cur).start.column = (cur).end.column = YYRHSLOC(rhs, 0).end.column; \
	}                                                                            \
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
	int                   val;
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
%nterm <ast> generic_selection
%nterm <ast> generic_assoc_list
%nterm <ast> generic_association
%nterm <ast> postfix_expression
%nterm <ast> unary_expression
%nterm <val> unary_operator
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
%nterm <val> assignment_operator
%nterm <ast> expression
%nterm <ast> constant_expression
%nterm <ast> storage_class_specifier
%nterm <ast> type_specifier
%nterm <ast> specifier_qualifier_list
%nterm <ast> type_qualifier
%nterm <ast> pointer
%nterm <ast> type_qualifier_list
%nterm <ast> type_name
%nterm <ast> static_assert_declaration


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
| generic_selection {
	TRACE("primary-expression", "generic-selection");
	$primary_expression = $generic_selection;
}
;


// 6.5.1.1
generic_selection: KEYWORD__GENERIC PUNCTUATOR_LPARENTHESIS assignment_expression[expression] PUNCTUATOR_COMMA generic_assoc_list[list] PUNCTUATOR_RPARENTHESIS {
	TRACE("generic-selection", "_Generic ( assignment-expression , generic-assoc-list )");

	$generic_selection = ast_generic_selection_init(
		$expression,
		$list,
		&@KEYWORD__GENERIC,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$generic_selection) YYNOMEM;
}


// 6.5.1.1
generic_assoc_list:
  generic_association {
	TRACE("generic-assoc-list", "generic-association");

	$generic_assoc_list = ast_generic_association_list_init(
		$generic_association,
		&@generic_association);
	if (!$generic_association) YYNOMEM;
}
| generic_assoc_list[child] PUNCTUATOR_COMMA generic_association {
	TRACE("generic-association", "generic-assoc-list : generic-association");

	$$ = ast_generic_association_list_append(
		$child,
		$generic_association,
		&@generic_association,
		&parser->error);
	ERROR($$);
}
;


// 6.5.1.1
generic_association:
  type_name[type] PUNCTUATOR_CONDITIONAL_COLON assignment_expression[expression] {
	TRACE("generic-association", "type-name : assignment-expression");

	$generic_association = ast_generic_association_init(
		$type,
		$expression,
		&@type,
		&@expression);
	if (!$generic_association) YYNOMEM;
}
| KEYWORD_DEFAULT PUNCTUATOR_CONDITIONAL_COLON assignment_expression {
	TRACE("generic-association", "default : assignment-expression");
	$generic_association = $assignment_expression;
}
;


// 6.5.2
postfix_expression:
  primary_expression {
	TRACE("postfix-expression", "primary-expression");
	$postfix_expression = $primary_expression;
}
/*
| postfix_expression PUNCTUATOR_LBRACKET expression PUNCTUATOR_RBRACKET {
	TRACE("postfix-expression", "postfix-expression [ expression ]");
}
| postfix_expression[child] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix-expression", "postfix-expression ( )");
}
| postfix_expression PUNCTUATOR_LPARENTHESIS argument_expression_list PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix-expression", "postfix-expression ( argument-expression-list )");
}
| postfix_expression[child] PUNCTUATOR_MEMBER_ACCESS identifier {
	TRACE("postfix-expression", "postfix-expression . identifier");
}
| postfix_expression PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE identifier {
	TRACE("postfix-expression", "postfix-expression -> identifier");
}
*/
| postfix_expression[operand] PUNCTUATOR_INCREMENT {
	TRACE("postfix-expression", "postfix-expression ++");

	$$ = ast_unary_operator_init(
		$operand,
		AST_UNARY_OPERATOR_POST_INCREMENT,
		&@operand,
		&@PUNCTUATOR_INCREMENT);
	if (!$$) YYNOMEM;
}
| postfix_expression[operand] PUNCTUATOR_DECREMENT {
	TRACE("postfix-expression", "postfix-expression --");

	$$ = ast_unary_operator_init(
		$operand,
		AST_UNARY_OPERATOR_POST_DECREMENT,
		&@operand,
		&@PUNCTUATOR_DECREMENT);
	if (!$$) YYNOMEM;
}
/*
| PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS PUNCTUATOR_LBRACE initializer_list PUNCTUATOR_RBRACE {
	TRACE("postfix-expression", "( type-name ) { initializer-list }");
}
| PUNCTUATOR_LPARENTHESIS type_name PUNCTUATOR_RPARENTHESIS PUNCTUATOR_LBRACKET initializer_list PUNCTUATOR_COMMA PUNCTUATOR_RBRACKET {
	TRACE("postfix-expression", "( type-name ) { initializer-list , }");
}
*/
;


// 6.5.3
unary_expression:
  postfix_expression {
	TRACE("unary-expression", "postfix-expression");
	$unary_expression = $postfix_expression;
}
| PUNCTUATOR_INCREMENT unary_expression[operand] {
	TRACE("unary-expression", "++ unary-expression");

	$$ = ast_unary_operator_init(
		$operand,
		AST_UNARY_OPERATOR_PRE_INCREMENT,
		&@PUNCTUATOR_INCREMENT,
		&@operand);
	if (!$$) YYNOMEM;
}
| PUNCTUATOR_DECREMENT unary_expression[operand] {
	TRACE("unary-expression", "-- unary-expression");

	$$ = ast_unary_operator_init(
		$operand,
		AST_UNARY_OPERATOR_PRE_DECREMENT,
		&@PUNCTUATOR_DECREMENT,
		&@operand);
	if (!$$) YYNOMEM;
}
| unary_operator[operator] cast_expression[operand] {
	TRACE("unary-expression", "unary-operator cast-expression");

	$unary_expression = ast_unary_operator_init(
		$operand,
		$operator,
		&@operator,
		&@operand);
	if (!$unary_expression) YYNOMEM;
}
| KEYWORD_SIZEOF unary_expression[operand] {
	TRACE("unary-expression", "sizeof unary-expression");

	$$ = ast_sizeof_init(
		$operand,
		&@KEYWORD_SIZEOF,
		&@operand);
	if (!$$) YYNOMEM;
}
| KEYWORD_SIZEOF PUNCTUATOR_LPARENTHESIS type_name[operand] PUNCTUATOR_RPARENTHESIS {
	TRACE("unary-expression", "sizeof ( type-name )");

	$unary_expression = ast_sizeof_init(
		$operand,
		&@KEYWORD_SIZEOF,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$unary_expression) YYNOMEM;
}
| KEYWORD__ALIGNOF PUNCTUATOR_LPARENTHESIS type_name[operand] PUNCTUATOR_RPARENTHESIS {
	TRACE("unary-expression", "_Alignof ( type-name )");

	$unary_expression = ast_alignof_init(
		$operand,
		&@KEYWORD__ALIGNOF,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$unary_expression) YYNOMEM;
}
;


// 6.5.3
unary_operator:
  PUNCTUATOR_AMPERSAND {
	TRACE("unary-operator", "&");
	$unary_operator = AST_UNARY_OPERATOR_AMPERSAND;
}
| PUNCTUATOR_ASTERISK {
	TRACE("unary-operator", "*");
	$unary_operator = AST_UNARY_OPERATOR_ASTERISK;
}
| PUNCTUATOR_PLUS {
	TRACE("unary-operator", "+");
	$unary_operator = AST_UNARY_OPERATOR_PLUS;
}
| PUNCTUATOR_MINUS {
	TRACE("unary-operator", "-");
	$unary_operator = AST_UNARY_OPERATOR_MINUS;
}
| PUNCTUATOR_UNARY_COMPLEMENT {
	TRACE("unary-operator", "~");
	$unary_operator = AST_UNARY_OPERATOR_UNARY_COMPLEMENT;
}
| PUNCTUATOR_LOGICAL_NOT {
	TRACE("unary-operator", "!");
	$unary_operator = AST_UNARY_OPERATOR_LOGICAL_NOT;
}
;


// 6.5.4
cast_expression:
  unary_expression {
	TRACE("cast-expression", "unary-expression");
	$cast_expression = $unary_expression;
}
| PUNCTUATOR_LPARENTHESIS type_name[type] PUNCTUATOR_RPARENTHESIS cast_expression[expression] {
	TRACE("cast-expression", "( type-name ) cast-expression");

	$$ = ast_cast_init(
		$expression,
		$type,
		&@PUNCTUATOR_LPARENTHESIS,
		&@expression);
	if (!$$) YYNOMEM;
}
;


// 6.5.4
multiplicative_expression:
  cast_expression {
	TRACE("multiplicative-expression", "cast-expression");
	$multiplicative_expression = $cast_expression;
}
| multiplicative_expression[lhs] PUNCTUATOR_ASTERISK cast_expression[rhs] {
	TRACE("multiplicative-expression", "multiplicative-expression * cast-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_MULTIPLICATION,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[lhs] PUNCTUATOR_DIVISION cast_expression[rhs] {
	TRACE("multiplicative-expression", "multiplicative-expression / cast-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_DIVISION,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| multiplicative_expression[lhs] PUNCTUATOR_MODULO cast_expression[rhs] {
	TRACE("multiplicative-expression", "multiplicative-expression % cast-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_MODULO,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.6
additive_expression:
  multiplicative_expression {
	TRACE("additive-expression", "multiplicative-expression");
	$additive_expression = $multiplicative_expression;
}
| additive_expression[lhs] PUNCTUATOR_PLUS multiplicative_expression[rhs] {
	TRACE("additive-expression", "additive-expression + multiplicative-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_ADDITION,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| additive_expression[lhs] PUNCTUATOR_MINUS multiplicative_expression[rhs] {
	TRACE("additive-expression", "additive-expression - multiplicative-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_SUBTRACTION,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.7
shift_expression:
  additive_expression {
	TRACE("shift-expression", "additive-expression");
	$shift_expression = $additive_expression;
}
| shift_expression[lhs] PUNCTUATOR_LBITSHIFT additive_expression[rhs] {
	TRACE("shift-expression", "shift-expression << additive-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_LBITSHIFT,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| shift_expression[lhs] PUNCTUATOR_RBITSHIFT additive_expression[rhs] {
	TRACE("shift-expression", "shift-expression >> additive-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_RBITSHIFT,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.8
relational_expression:
  shift_expression {
	TRACE("relational-expression", "shift-expression");
	$relational_expression = $shift_expression;
}
| relational_expression[lhs] PUNCTUATOR_LESS_THAN shift_expression[rhs] {
	TRACE("relational-expression", "relational-expression < shift-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_LESS_THAN,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| relational_expression[lhs] PUNCTUATOR_GREATER_THAN shift_expression[rhs] {
	TRACE("relational-expression", "relational-expression > shift-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_GREATER_THAN,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| relational_expression[lhs] PUNCTUATOR_LESS_THAN_OR_EQUAL shift_expression[rhs] {
	TRACE("relational-expression", "relational-expression <= shift-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_LESS_THAN_OR_EQUAL,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| relational_expression[lhs] PUNCTUATOR_GREATER_THAN_OR_EQUAL shift_expression[rhs] {
	TRACE("relational-expression", "relational-expression >= shift-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_GREATER_THAN_OR_EQUAL,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.9
equality_expression:
  relational_expression {
	TRACE("equality-expression", "relational-expression");
	$equality_expression = $relational_expression;
}
| equality_expression[lhs] PUNCTUATOR_EQUALITY relational_expression[rhs] {
	TRACE("equality-expression", "equality-expression == relational-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_EQUALITY,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
| equality_expression[lhs] PUNCTUATOR_INEQUALITY relational_expression[rhs] {
	TRACE("equality-expression", "equality-expression != relational-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_INEQUALITY,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.10
and_expression:
  equality_expression {
	TRACE("AND-expression", "equality-expression");
	$and_expression = $equality_expression;
}
| and_expression[lhs] PUNCTUATOR_AMPERSAND equality_expression[rhs] {
	TRACE("AND-expression", "AND-expression & equality-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_AND,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.11
exclusive_or_expression:
  and_expression {
	TRACE("exclusive-OR-expression", "AND-expression");
	$exclusive_or_expression = $and_expression;
}
| exclusive_or_expression[lhs] PUNCTUATOR_XOR and_expression[rhs] {
	TRACE("exclusive-OR-expression", "exclusive-OR-expression ^ AND-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_EXCLUSIVE_OR,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.12
inclusive_or_expression:
  exclusive_or_expression {
	TRACE("inclusive-OR-expression", "exclusive-OR-expression");
	$inclusive_or_expression = $exclusive_or_expression;
}
| inclusive_or_expression[lhs] PUNCTUATOR_OR exclusive_or_expression[rhs] {
	TRACE("inclusive-OR-expression", "inclusive-OR-expression | exclusive-OR-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_INCLUSIVE_OR,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.13
logical_and_expression:
  inclusive_or_expression {
	TRACE("logical-AND-expression", "inclusive-OR-expression");
	$logical_and_expression = $inclusive_or_expression;
}
| logical_and_expression[lhs] PUNCTUATOR_LOGICAL_AND inclusive_or_expression[rhs] {
	TRACE("logical-AND-expression", "logical-AND-expression && inclusive-OR-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_LOGICAL_AND,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.14
logical_or_expression:
  logical_and_expression {
	TRACE("logical-OR-expression", "logical-AND-expression");
	$logical_or_expression = $logical_and_expression;
}
| logical_or_expression[lhs] PUNCTUATOR_LOGICAL_OR logical_and_expression[rhs] {
	TRACE("logical-OR-expression", "logical-OR-expression || logical-AND-expression");

	$$ = ast_binary_operator_init(
		$lhs,
		$rhs,
		AST_BINARY_OPERATOR_LOGICAL_OR,
		&@lhs,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.15
conditional_expression:
  logical_or_expression {
	TRACE("conditional-expression", "logical-OR-expression");
	$conditional_expression = $logical_or_expression;
}
| logical_or_expression[condition] PUNCTUATOR_CONDITIONAL_QUESTION expression[lhs] PUNCTUATOR_CONDITIONAL_COLON conditional_expression[rhs] {
	TRACE("conditional-expression", "logical-OR-expression ? expression : conditional-expression");

	$$ = ast_ternary_operator_init(
		$condition,
		$lhs,
		$rhs,
		&@condition,
		&@rhs);
	if (!$$) YYNOMEM;
}
;


// 6.5.16
assignment_expression:
  conditional_expression {
	TRACE("assignment-expression", "conditional-expression");
	$assignment_expression = $conditional_expression;
}
| unary_expression[lvalue] assignment_operator[operator] assignment_expression[rvalue] {
	TRACE("assignment-expression", "unary-expression assignment-operator assignment-expression");

	$$ = ast_assignment_init(
		$lvalue,
		$rvalue,
		$operator,
		&@lvalue,
		&@rvalue);
	if (!$$) YYNOMEM;
}
;


// 6.5.16
assignment_operator:
  PUNCTUATOR_ASSIGNMENT {
	TRACE("assignment-operator", "=");
	$assignment_operator = AST_ASSIGNMENT_ASSIGNMENT;
}
| PUNCTUATOR_COMPOUND_MULTIPLICATION {
	TRACE("assignment-operator", "*=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_MULTIPLICATION;
}
| PUNCTUATOR_COMPOUND_DIVISION {
	TRACE("assignment-operator", "/=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_DIVISION;
}
| PUNCTUATOR_COMPOUND_MODULO {
	TRACE("assignment-operator", "%=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_MODULO;
}
| PUNCTUATOR_COMPOUND_ADDITION {
	TRACE("assignment-operator", "+=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_ADDITION;
}
| PUNCTUATOR_COMPOUND_SUBTRACTION {
	TRACE("assignment-operator", "-=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_SUBTRACTION;
}
| PUNCTUATOR_COMPOUND_LBITSHIFT {
	TRACE("assignment-operator", "<<=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_LBITSHIFT;
}
| PUNCTUATOR_COMPOUND_RBITSHIFT {
	TRACE("assignment-operator", ">>=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_RBITSHIFT;
}
| PUNCTUATOR_COMPOUND_AND {
	TRACE("assignment-operator", "&=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_AND;
}
| PUNCTUATOR_COMPOUND_XOR {
	TRACE("assignment-operator", "^=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_XOR;
}
| PUNCTUATOR_COMPOUND_OR {
	TRACE("assignment-operator", "|=");
	$assignment_operator = AST_ASSIGNMENT_COMPOUND_OR;
}
;


// 6.5.17
expression:
  assignment_expression {
	TRACE("expression", "assignment-expression");
	$expression = $assignment_expression;
}
| expression[parent_expression] PUNCTUATOR_COMMA assignment_expression {
	TRACE("expression", "expression , assignment-expression");

	$$ = (*$parent_expression == AST_EXPRESSION)
		? ast_expression_append(
			$parent_expression,
			$assignment_expression,
			&@assignment_expression)
		: ast_expression_init(
			$parent_expression,
			$assignment_expression,
			&@parent_expression,
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
	TRACE("storage-class-specifier", "typedef");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_TYPEDEF,
		&@KEYWORD_TYPEDEF);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_EXTERN {
	TRACE("storage-class-specifier", "extern");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_EXTERN,
		&@KEYWORD_EXTERN);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_STATIC {
	TRACE("storage-class-specifier", "static");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_STATIC,
		&@KEYWORD_STATIC);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD__THREAD_LOCAL {
	TRACE("storage-class-specifier", "_Thread_local");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER__THREAD_LOCAL,
		&@KEYWORD__THREAD_LOCAL);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_AUTO {
	TRACE("storage-class-specifier", "auto");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_AUTO,
		&@KEYWORD_AUTO);
	if (!$storage_class_specifier) YYNOMEM;
}
| KEYWORD_REGISTER {
	TRACE("storage-class-specifier", "register");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_REGISTER,
		&@KEYWORD_REGISTER);
	if (!$storage_class_specifier) YYNOMEM;
}
;


// 6.7.2
type_specifier:
  KEYWORD_VOID {
	TRACE("type-specifier", "void");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_VOID,
		NULL,
		&@KEYWORD_VOID);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_CHAR {
	TRACE("type-specifier", "char");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_CHAR,
		NULL,
		&@KEYWORD_CHAR);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_SHORT {
	TRACE("type-specifier", "short");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_SHORT,
		NULL,
		&@KEYWORD_SHORT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_INT {
	TRACE("type-specifier", "int");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_INT,
		NULL,
		&@KEYWORD_INT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_LONG {
	TRACE("type-specifier", "long");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_LONG,
		NULL,
		&@KEYWORD_LONG);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_FLOAT {
	TRACE("type-specifier", "float");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_FLOAT,
		NULL,
		&@KEYWORD_FLOAT);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_DOUBLE {
	TRACE("type-specifier", "double");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_DOUBLE,
		NULL,
		&@KEYWORD_DOUBLE);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_SIGNED {
	TRACE("type-specifier", "signed");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_SIGNED,
		NULL,
		&@KEYWORD_SIGNED);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD_UNSIGNED {
	TRACE("type-specifier", "unsigned");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_UNSIGNED,
		NULL,
		&@KEYWORD_UNSIGNED);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD__BOOL {
	TRACE("type-specifier", "_Bool");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER__BOOL,
		NULL,
		&@KEYWORD__BOOL);
	if (!$type_specifier) YYNOMEM;
}
| KEYWORD__COMPLEX {
	TRACE("type-specifier", "_Complex");

	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER__COMPLEX,
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
	TRACE("specifier-qualifier-list", "type-specifier");

	$specifier_qualifier_list = ast_specifier_qualifier_list_init(
		$type_specifier,
		NULL,
		&@type_specifier);
	if (!$specifier_qualifier_list) YYNOMEM;
}
| type_specifier specifier_qualifier_list[list] {
	TRACE("specifier-qualifier-list", "type-specifier specifier-qualifier-list");

	$$ = ast_specifier_qualifier_list_append(
		$list,
		$type_specifier,
		&@type_specifier,
		&parser->error);
	ERROR($$);
}
| type_qualifier {
	TRACE("specifier-qualifier-list", "type-qualifier");

	$specifier_qualifier_list = ast_specifier_qualifier_list_init(
		NULL,
		$type_qualifier,
		&@type_qualifier);
	if (!$specifier_qualifier_list) YYNOMEM;
}
| type_qualifier specifier_qualifier_list[list] {
	TRACE("specifier-qualifier-list", "type-qualifier specifier-qualifier-list");

	$$ = ast_specifier_qualifier_list_append(
		$list,
		$type_qualifier,
		&@type_qualifier,
		&parser->error);
	ERROR($$);
}


// 6.7.3
type_qualifier:
  KEYWORD_CONST {
	TRACE("type-qualifier", "const");

	$type_qualifier = ast_type_qualifier_init(
		AST_TYPE_QUALIFIER_CONST,
		&@KEYWORD_CONST);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD_RESTRICT {
	TRACE("type-qualifier", "restrict");

	$type_qualifier = ast_type_qualifier_init(
		AST_TYPE_QUALIFIER_RESTRICT,
		&@KEYWORD_RESTRICT);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD_VOLATILE {
	TRACE("type-qualifier", "volatile");

	$type_qualifier = ast_type_qualifier_init(
		AST_TYPE_QUALIFIER_VOLATILE,
		&@KEYWORD_VOLATILE);
	if (!$type_qualifier) YYNOMEM;
}
| KEYWORD__ATOMIC {
	TRACE("type-qualifier", "_Atomic");

	$type_qualifier = ast_type_qualifier_init(
		AST_TYPE_QUALIFIER__ATOMIC,
		&@KEYWORD__ATOMIC);
	if (!$type_qualifier) YYNOMEM;
}
;


// 6.7.6
pointer:
  PUNCTUATOR_ASTERISK {
	TRACE("pointer", "*");

	$pointer = ast_pointer_init(NULL, NULL, &@PUNCTUATOR_ASTERISK);
	if (!$pointer) YYNOMEM;
}
| PUNCTUATOR_ASTERISK type_qualifier_list {
	TRACE("pointer", "* type-qualifier-list");

	$pointer = ast_pointer_init(
		NULL,
		$type_qualifier_list,
		&@PUNCTUATOR_ASTERISK);
	if (!$pointer) YYNOMEM;
}
| PUNCTUATOR_ASTERISK pointer[child] {
	TRACE("pointer", "* pointer");

	$$ = ast_pointer_init(NULL, $child, &@PUNCTUATOR_ASTERISK);
	if (!$$) YYNOMEM;
}
| PUNCTUATOR_ASTERISK type_qualifier_list pointer[child] {
	TRACE("pointer", "* type-qualifier-list pointer");

	$$ = ast_pointer_init(
		$child,
		$type_qualifier_list,
		&@PUNCTUATOR_ASTERISK);
	if (!$$) YYNOMEM;
}
;


// 6.7.6
type_qualifier_list:
  type_qualifier {
	TRACE("type-qualifier-list", "type-qualifier");

	$type_qualifier_list = ast_type_qualifier_list_init(
		$type_qualifier,
		&@type_qualifier);
	if (!$type_qualifier_list) YYNOMEM;
}
| type_qualifier_list[list] type_qualifier {
	TRACE("type-qualifier-list", "type-qualifier-list type-qualifier");

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
	TRACE("type-name", "specifier-qualifier-list");

	$type_name = ast_type_name_init(
		$specifier_qualifier_list,
		NULL,
		&@specifier_qualifier_list,
		NULL);
	if (!$type_name) YYNOMEM;
}
/*
| specifier_qualifier_list abstract_declarator {
	TRACE("type-name", "specifier-qualifier-list abstract-declarator");

	$type_name = ast_type_name_init(
		$specifier_qualifier_list,
		$abstract_declarator,
		&@specifier_qualifier_list,
		&@abstract_declarator);
	if (!$type_name) YYNOMEM;
}
*/
;


// 6.7.10
static_assert_declaration: KEYWORD__STATIC_ASSERT PUNCTUATOR_LPARENTHESIS constant_expression PUNCTUATOR_COMMA string_literal PUNCTUATOR_RPARENTHESIS PUNCTUATOR_SEMICOLON {
	TRACE("static_assert_declaration", "_Static_assert ( constant-expression , string-literal ) ;");

	$static_assert_declaration = ast_static_assert_init(
		$constant_expression,
		$string_literal,
		&@KEYWORD__STATIC_ASSERT,
		&@PUNCTUATOR_SEMICOLON);
	if (!$static_assert_declaration) YYNOMEM;
}
