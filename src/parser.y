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
#include <jkcc/constant.h>
#include <jkcc/lexer.h>
#include <jkcc/location.h>
#include <jkcc/parser.h>
#include <jkcc/scope.h>
#include <jkcc/string.h>
#include <jkcc/symbol.h>
#include <jkcc/trace.h>


#define ERROR_ADDR &parser->yyextra_data->error

#define ERROR_RESET parser->yyextra_data->error = NULL

#define ERROR(ast) {                                          \
	if (!ast) {                                           \
		if (parser->yyextra_data->error) {            \
			yyerror(                              \
				&yylloc,                      \
				scanner,                      \
				parser,                       \
				translation_unit,             \
				parser->yyextra_data->error); \
			YYERROR;                              \
		} else {                                      \
			YYNOMEM;                              \
		}                                             \
	}                                                     \
}

#define APPEND_BASE_TYPE(ast_type) {                                \
	if (vector_append(&translation_unit->base_type, &ast_type)) \
		YYNOMEM;                                            \
}

#define CHECK_IDENTIFIER_COLLISION(ast_identifier) {                        \
	if (symbol_check_identifier_collision(                              \
		translation_unit->symbol_table->context.current.identifier, \
		ast_identifier))                                            \
	{                                                                   \
		yyerror(                                                    \
			&yylloc,                                            \
			scanner,                                            \
			parser,                                             \
			translation_unit,                                   \
			"redeclaration of identifier");                     \
		YYERROR;                                                    \
	}                                                                   \
}

#define GET_CURRENT_IDENTIFIER parser->yyextra_data->identifier

#define GET_BASE_STORAGE_CLASS parser->yyextra_data->symbol_table->context.base.storage_class

#define GET_BASE_TYPE parser->yyextra_data->symbol_table->context.base.type

#define GET_CURRENT_STORAGE_CLASS                                                   \
	parser->yyextra_data->symbol_table->context.current.storage_class =         \
		(parser->yyextra_data->symbol_table->context.current.storage_class) \
		? parser->yyextra_data->symbol_table->context.current.storage_class \
		: parser->yyextra_data->symbol_table->context.base.storage_class    \

#define GET_CURRENT_TYPE                                                   \
	parser->yyextra_data->symbol_table->context.current.type =         \
		(parser->yyextra_data->symbol_table->context.current.type) \
		? parser->yyextra_data->symbol_table->context.current.type \
		: parser->yyextra_data->symbol_table->context.base.type    \

#define GET_IDENTIFIER_SYMBOL_TABLE(symbol, ast_identifier) {                                \
	switch (parser->yyextra_data->identifier_type) {                                     \
		case AST_TYPE_SPECIFIER_STRUCT_OR_UNION_SPECIFIER:                           \
			symbol = translation_unit->symbol_table->context.current.tag;        \
			break;                                                               \
                                                                                             \
		default:                                                                     \
			symbol = translation_unit->symbol_table->context.current.identifier; \
			break;                                                               \
	}                                                                                    \
                                                                                             \
	parser->yyextra_data->identifier_type = 0;                                           \
}

#define GET_IDENTIFIER_TYPE(symbol, ast_identifier, ast_type)   \
	symbol_get_identifier(symbol, ast_identifier, ast_type)

#define GET_VARIADIC_PARAMETER parser->yyextra_data->variadic_parameter

#define SCOPE_POP scope_pop(translation_unit->symbol_table)

#define SET_BASE_TYPE(ast_type) {                                            \
	parser->yyextra_data->symbol_table->context.current.type = NULL;     \
	parser->yyextra_data->symbol_table->context.base.type    = ast_type; \
}

#define SET_CURRENT_STORAGE_CLASS(new_storage_class) {                                         \
	parser->yyextra_data->symbol_table->context.current.storage_class = new_storage_class; \
}

#define SET_CURRENT_TYPE(ast_type) {                                         \
	parser->yyextra_data->symbol_table->context.current.type = ast_type; \
}

#define SET_CURRENT_IDENTIFIER(ast_identifier) {           \
	parser->yyextra_data->identifier = ast_identifier; \
}

#define SET_VARIADIC_PARAMETER(variadic) {                   \
	parser->yyextra_data->variadic_parameter = variadic; \
}

#define STRUCT_INSTALL_TAG(ast_struct, struct_or_union, identifier, struct_or_union_yylloc, identifier_yylloc) { \
	ast_t *struct_type = ast_struct_init(                                                                    \
		identifier,                                                                                      \
		NULL,                                                                                            \
		NULL,                                                                                            \
		struct_or_union,                                                                                 \
		struct_or_union_yylloc,                                                                          \
		identifier_yylloc);                                                                              \
	if (!struct_type) YYNOMEM;                                                                               \
                                                                                                                 \
	switch (symbol_insert_tag(                                                                               \
		translation_unit->symbol_table->context.current.tag,                                             \
		identifier,                                                                                      \
		struct_type)                                                                                     \
	) {                                                                                                      \
		case SYMBOL_ERROR_NOMEM:                                                                         \
			YYNOMEM;                                                                                 \
			break;                                                                                   \
                                                                                                                 \
		case SYMBOL_ERROR_EXISTS:                                                                        \
			yyerror(                                                                                 \
				&yylloc,                                                                         \
				scanner,                                                                         \
				parser,                                                                          \
				translation_unit,                                                                \
				"redeclaration of struct or union");                                             \
			YYERROR;                                                                                 \
			break;                                                                                   \
                                                                                                                 \
		default:                                                                                         \
			break;                                                                                   \
	}                                                                                                        \
                                                                                                                 \
	ast_struct = struct_type;                                                                                \
}

#define RESET_BASE_STORAGE_CLASS {                         \
	SET_CURRENT_STORAGE_CLASS(GET_BASE_STORAGE_CLASS); \
}

#define RESET_BASE_TYPE SET_CURRENT_TYPE(GET_BASE_TYPE)

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
#include <jkcc/constant.h>
#include <jkcc/lexer.h>
#include <jkcc/parser.h>


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
%nterm <ast> argument_expression_list
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
%nterm <ast> declaration
%nterm <ast> declaration_specifiers
%nterm <ast> storage_class_specifier
%nterm <ast> init_declarator_list
%nterm <ast> init_declarator
%nterm <ast> type_specifier
%nterm <ast> struct_or_union_specifier
%nterm <val> struct_or_union
%nterm <ast> struct_declaration_list
%nterm <ast> struct_declaration
%nterm <ast> specifier_qualifier_list
%nterm <ast> struct_declarator_list
%nterm <ast> struct_declarator
%nterm <ast> type_qualifier
%nterm <ast> function_specifier
%nterm <ast> alignment_specifier
%nterm <ast> declarator
%nterm <ast> direct_declarator
%nterm <ast> pointer
%nterm <ast> type_qualifier_list
%nterm <ast> parameter_type_list
%nterm <ast> parameter_list
%nterm <ast> parameter_declaration
%nterm <ast> identifier_list
%nterm <ast> type_name
%nterm <ast> atomic_type_specifier
%nterm <ast> abstract_declarator
%nterm <ast> direct_abstract_declarator
%nterm <ast> static_assert_declaration

%nterm <ast> struct_install_tag


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

	CHECK_IDENTIFIER_COLLISION($identifier);

	symbol_table_t *symbol;
	GET_IDENTIFIER_SYMBOL_TABLE(symbol, $identifier)

	ast_t *type;
	if (!GET_IDENTIFIER_TYPE(symbol, $identifier, &type))
		ast_identifier_set_type($identifier, type);

	SET_CURRENT_IDENTIFIER($identifier);
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

	ERROR_RESET;

	$$ = ast_generic_association_list_append(
		$child,
		$generic_association,
		&@generic_association,
		ERROR_ADDR);
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
| postfix_expression[operand] PUNCTUATOR_LBRACKET expression PUNCTUATOR_RBRACKET {
	TRACE("postfix-expression", "postfix-expression [ expression ]");

	ast_t *expression = ast_binary_operator_init(
		$operand,
		$expression,
		AST_BINARY_OPERATOR_ADDITION,
		&@operand,
		&@PUNCTUATOR_RBRACKET);
	if (!expression) YYNOMEM;

	$$ = ast_dereference_init(
		expression,
		&@operand,
		&@PUNCTUATOR_RBRACKET);
	if (!$$) YYNOMEM;
}
| postfix_expression[expression] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix-expression", "postfix-expression ( )");

	$$ = ast_call_init(
		$expression,
		NULL,
		&@expression,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$$) YYNOMEM;
}
| postfix_expression[expression] PUNCTUATOR_LPARENTHESIS argument_expression_list[argument_list] PUNCTUATOR_RPARENTHESIS {
	TRACE("postfix-expression", "postfix-expression ( argument-expression-list )");

	$$ = ast_call_init(
		$expression,
		$argument_list,
		&@expression,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$$) YYNOMEM;
}
| postfix_expression[operand] PUNCTUATOR_MEMBER_ACCESS identifier {
	TRACE("postfix-expression", "postfix-expression . identifier");

	$$ = ast_member_access_init(
		$operand,
		$identifier,
		&@operand,
		&@identifier);
	if (!$$) YYNOMEM;
}
| postfix_expression[operand] PUNCTUATOR_MEMBER_ACCESS_DEREFERENCE identifier {
	TRACE("postfix-expression", "postfix-expression -> identifier");

	ast_t *operand = ast_dereference_init(
		$operand,
		&@operand,
		&@identifier);
	if (!operand) YYNOMEM;

	$$ = ast_member_access_init(
		operand,
		$identifier,
		&@operand,
		&@identifier);
	if (!$$) YYNOMEM;
}
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


// 6.5.2
argument_expression_list:
  assignment_expression[argument] {
	TRACE("argument-expression-list", "assignment-expression");

	$argument_expression_list = ast_argument_list_init(
		$argument,
		&@argument);
	if (!$argument_expression_list) YYNOMEM;
}
| argument_expression_list[list] PUNCTUATOR_COMMA assignment_expression[argument] {
	TRACE("argument-expression-list", "argument-expression-list , assignment-expression");

	$$ = ast_argument_list_append(
		$list,
		$argument,
		&@argument);
	if (!$$) YYNOMEM;
}
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

	switch ($operator) {
		case AST_UNARY_OPERATOR_AMPERSAND:
			$unary_expression = ast_addressof_init(
				$operand,
				&@operator,
				&@operand);
			break;

		case AST_UNARY_OPERATOR_ASTERISK:
			$unary_expression = ast_dereference_init(
				$operand,
				&@operator,
				&@operand);
			break;

		default:
			$unary_expression = ast_unary_operator_init(
				$operand,
				$operator,
				&@operator,
				&@operand);
			break;
	}
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


// 6.7
declaration:
  declaration_specifiers PUNCTUATOR_SEMICOLON {
	TRACE("declaration", "declaration-specifiers ;");
	$declaration = $declaration_specifiers;

	APPEND_BASE_TYPE($declaration_specifiers);
	RESET_BASE_STORAGE_CLASS;
}
| declaration_specifiers init_declarator_list PUNCTUATOR_SEMICOLON {
	TRACE("declaration", "declaration-specifiers init-declarator-list ;");
	$declaration = $init_declarator_list;

	APPEND_BASE_TYPE($declaration_specifiers);
	RESET_BASE_STORAGE_CLASS;
}
| static_assert_declaration {
	TRACE("declaration", "static_assert-declaration");
	$declaration = $static_assert_declaration;
}
;


// 6.7
declaration_specifiers:
  storage_class_specifier {
	TRACE("declaration-specifiers", "storage-class-specifier");

	$declaration_specifiers = ast_type_init(
		$storage_class_specifier,
		&@storage_class_specifier);
	if (!$declaration_specifiers) YYNOMEM;

	SET_BASE_TYPE($declaration_specifiers);
}
| storage_class_specifier declaration_specifiers[type] {
	TRACE("declaration-specifiers", "storage-class-specifier declaration-specifiers");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$storage_class_specifier,
		&@storage_class_specifier,
		ERROR_ADDR);
	ERROR($$);

	SET_BASE_TYPE($$);
}
| type_specifier {
	TRACE("declaration-specifiers", "type-specifier");

	$declaration_specifiers = ast_type_init(
		$type_specifier,
		&@type_specifier);
	if (!$declaration_specifiers) YYNOMEM;

	SET_BASE_TYPE($declaration_specifiers);
}
| type_specifier declaration_specifiers[type] {
	TRACE("declaration-specifiers", "type-specifier declaration-specifiers");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$type_specifier,
		&@type_specifier,
		ERROR_ADDR);
	ERROR($$);

	SET_BASE_TYPE($$);
}
| type_qualifier {
	TRACE("declaration-specifiers", "type-qualifier");

	$declaration_specifiers = ast_type_init(
		$type_qualifier,
		&@type_qualifier);
	if (!$declaration_specifiers) YYNOMEM;

	SET_BASE_TYPE($declaration_specifiers);
}
| type_qualifier declaration_specifiers[type] {
	TRACE("declaration-specifiers", "type-qualifier declaration-specifiers");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$type_qualifier,
		&@type_qualifier,
		ERROR_ADDR);
	ERROR($$);

	SET_BASE_TYPE($$);
}
| function_specifier {
	TRACE("declaration-specifiers", "function-specifier");

	$declaration_specifiers = ast_type_init(
		$function_specifier,
		&@function_specifier);
	if (!$declaration_specifiers) YYNOMEM;

	SET_BASE_TYPE($declaration_specifiers);
}
| function_specifier declaration_specifiers[type] {
	TRACE("declaration-specifiers", "function-specifier declaration-specifiers");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$function_specifier,
		&@function_specifier,
		ERROR_ADDR);
	ERROR($$);

	SET_BASE_TYPE($$);
}
| alignment_specifier {
	TRACE("declaration-specifiers", "alignment-specifier");

	$declaration_specifiers = ast_type_init(
		$alignment_specifier,
		&@alignment_specifier);
	if (!$declaration_specifiers) YYNOMEM;

	SET_BASE_TYPE($declaration_specifiers);
}
| alignment_specifier declaration_specifiers[type] {
	TRACE("declaration-specifiers", "alignment-specifier declaration-specifiers");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$alignment_specifier,
		&@alignment_specifier,
		ERROR_ADDR);
	ERROR($$);

	SET_BASE_TYPE($$);
}


// 6.7
init_declarator_list:
  init_declarator {
	TRACE("init-declarator-list", "init-declarator");
	$init_declarator_list = $init_declarator;
}
| init_declarator_list[list] PUNCTUATOR_COMMA init_declarator[declaration] {
	TRACE("init-declarator-list", "init-declarator-list , init-declarator");

	$$ = (*$list == AST_DECLARATION_LIST)
		? ast_declaration_list_append(
			$list,
			$declaration,
			&@declaration)
		: ast_declaration_list_init(
			$list,
			$declaration,
			&@list,
			&@declaration);
	if (!$$) YYNOMEM;
}
;



// 6.7
init_declarator:
  declarator[identifier] {
	TRACE("init-declarator", "declarator");

	ast_t *type = GET_CURRENT_TYPE;

	if (symbol_insert_identifier(
		translation_unit->symbol_table->context.current.identifier,
		$identifier,
		type)) YYNOMEM;

	$init_declarator = ast_declaration_init(
		type,
		$identifier,
		NULL,
		GET_CURRENT_STORAGE_CLASS,
		&@identifier,
		&@identifier);
	if (!$init_declarator) YYNOMEM;

	RESET_BASE_TYPE;
}
/*
| declarator[lvalue] PUNCTUATOR_ASSIGNMENT initializer[rvalue] {
	TRACE("init-declarator", "declarator = initializer");

	ast_t *type = GET_CURRENT_TYPE;
	ast_t *identifier = GET_CURRENT_IDENTIFIER;

	if (parse_insert_identifier(parser, identifier, type)) YYNOMEM;

	$init_declarator = ast_assignment_init(
		lvalue,
		rvalue,
		AST_ASSIGNMENT_ASSIGNMENT,
		&@lvalue,
		&@rvalue);
	if ($$) YYNOMEM;

	RESET_BASE_TYPE;
}
*/
;


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

	SET_CURRENT_STORAGE_CLASS(AST_DECLARATION_EXTERN);
}
| KEYWORD_STATIC {
	TRACE("storage-class-specifier", "static");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_STATIC,
		&@KEYWORD_STATIC);
	if (!$storage_class_specifier) YYNOMEM;

	SET_CURRENT_STORAGE_CLASS(AST_DECLARATION_STATIC);
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

	SET_CURRENT_STORAGE_CLASS(AST_DECLARATION_AUTO);
}
| KEYWORD_REGISTER {
	TRACE("storage-class-specifier", "register");

	$storage_class_specifier = ast_storage_class_specifier_init(
		AST_STORAGE_CLASS_SPECIFIER_REGISTER,
		&@KEYWORD_REGISTER);
	if (!$storage_class_specifier) YYNOMEM;

	SET_CURRENT_STORAGE_CLASS(AST_DECLARATION_REGISTER);
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
| struct_or_union_specifier {
	$type_specifier = ast_type_specifier_init(
		AST_TYPE_SPECIFIER_STRUCT_OR_UNION_SPECIFIER,
		$struct_or_union_specifier,
		&@struct_or_union_specifier);
	if (!$type_specifier) YYNOMEM;
}
// | enum_specifier
// | typedef_name
;


// 6.7.2.1
struct_or_union_specifier:
  struct_or_union PUNCTUATOR_LBRACKET struct_scope_push struct_declaration_list PUNCTUATOR_RBRACKET {
	TRACE("struct-or-union-specifier", "struct-or-union { struct-declaration-list }");

	$struct_or_union_specifier = ast_struct_init(
		NULL,
		$struct_declaration_list,
		parser->yyextra_data->symbol_table->context.current.identifier,
		$struct_or_union,
		&@struct_or_union,
		&@PUNCTUATOR_RBRACKET);
	if (!$struct_or_union_specifier) YYNOMEM;

	SCOPE_POP;
}
| struct_or_union identifier struct_install_tag PUNCTUATOR_LBRACE struct_scope_push struct_declaration_list PUNCTUATOR_RBRACE {
	TRACE("struct-or-union-specifier", "struct-or-union identifier { struct-declaration-list }");

	// handled by struct_install_tag
	(void) $struct_or_union;
	(void) $identifier;

	ast_t *ast_struct = $struct_install_tag;

	ast_struct_set_declaration_list(ast_struct, $struct_declaration_list);
	ast_struct_set_symbol_table(
		ast_struct,
		parser->yyextra_data->symbol_table->context.current.identifier);

	$struct_or_union_specifier = ast_struct;

	SCOPE_POP;
}
| struct_or_union identifier struct_install_tag {
	TRACE("struct-or-union-specifier", "struct-or-union identifier");

	// handled by struct_install_tag
	(void) $struct_or_union;
	(void) $identifier;

	$struct_or_union_specifier = $struct_install_tag;
}


// 6.7.2.1
struct_or_union:
  KEYWORD_STRUCT {
	TRACE("struct-or-union", "struct");
	$struct_or_union = AST_STRUCT_STRUCT;
}
| KEYWORD_UNION {
	TRACE("struct-or-union", "union");
	$struct_or_union = AST_STRUCT_UNION;
}
;


// 6.7.2.1
struct_declaration_list:
  struct_declaration {
	TRACE("struct-declaration-list", "struct-declaration");
	$struct_declaration = $struct_declaration_list;
}
| struct_declaration_list[list] struct_declaration[declaration] {
	TRACE("struct-declaration-list", "struct-declaration-list struct-declaration");

	$$ = (*$list == AST_DECLARATION_LIST)
		? ast_declaration_list_append(
			$list,
			$declaration,
			&@declaration)
		: ast_declaration_list_init(
			$list,
			$declaration,
			&@list,
			&@declaration);
	if (!$$) YYNOMEM;
}


// 6.7.2.1
struct_declaration:
  specifier_qualifier_list PUNCTUATOR_SEMICOLON {
	TRACE("struct-declaration", "specifier-qualifier-list ;");
	$struct_declaration = $specifier_qualifier_list;
}
| specifier_qualifier_list set_base_type struct_declarator_list PUNCTUATOR_SEMICOLON {
	TRACE("struct-declaration", "specifier-qualifier-list struct-declarator-list ;");
	$struct_declaration = $struct_declarator_list;

	APPEND_BASE_TYPE($specifier_qualifier_list);
	RESET_BASE_STORAGE_CLASS;
}
| static_assert_declaration {
	TRACE("struct-declaration", "static_assert-declaration");
	$struct_declaration = $static_assert_declaration;
}


// 6.7.2.1
specifier_qualifier_list:
  type_specifier {
	TRACE("specifier-qualifier-list", "type-specifier");

	$specifier_qualifier_list = ast_type_init(
		$type_specifier,
		&@type_specifier);
	if (!$specifier_qualifier_list) YYNOMEM;
}
| type_specifier specifier_qualifier_list[type] {
	TRACE("specifier-qualifier-list", "type-specifier specifier-qualifier-list");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$type_specifier,
		&@type_specifier,
		ERROR_ADDR);
	ERROR($$);
}
| type_qualifier {
	TRACE("specifier-qualifier-list", "type-qualifier");

	$specifier_qualifier_list = ast_type_init(
		$type_qualifier,
		&@type_qualifier);
	if (!$specifier_qualifier_list) YYNOMEM;
}
| type_qualifier specifier_qualifier_list[type] {
	TRACE("specifier-qualifier-list", "type-qualifier specifier-qualifier-list");

	ERROR_RESET;

	$$ = ast_type_append(
		$type,
		$type_qualifier,
		&@type_qualifier,
		ERROR_ADDR);
	ERROR($$);
}


// 6.7.2.1
struct_declarator_list:
  struct_declarator {
	TRACE("struct-declarator-list", "struct-declarator");
	$struct_declarator_list = $struct_declarator;
}
| struct_declarator_list[list] PUNCTUATOR_COMMA struct_declarator[declaration] {
	TRACE("struct-declarator-list", "struct-declarator-list , struct-declarator");

	$$ = (*$list == AST_DECLARATION_LIST)
		? ast_declaration_list_append(
			$list,
			$declaration,
			&@declaration)
		: ast_declaration_list_init(
			$list,
			$declaration,
			&@list,
			&@declaration);
	if (!$$) YYNOMEM;
}


// 6.7.2.1
struct_declarator:
  declarator[identifier] {
	TRACE("struct-declarator", "declarator");

	ast_t *type = GET_CURRENT_TYPE;

	if (symbol_insert_identifier(
		translation_unit->symbol_table->context.current.identifier,
		$identifier,
		type)) YYNOMEM;

	$struct_declarator = ast_declaration_init(
		type,
		$identifier,
		NULL,
		GET_CURRENT_STORAGE_CLASS,
		&@identifier,
		&@identifier);
	if (!$struct_declarator) YYNOMEM;

	RESET_BASE_TYPE;
}
/*
| PUNCTUATOR_CONDITIONAL_COLON constant_expression {
	TRACE("struct-declarator", ": constant-expression");
}
| declarator PUNCTUATOR_CONDITIONAL_COLON constant_expression {
	TRACE("struct-declarator", "declarator : constant-expression");
}
*/


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


// 6.7.4
function_specifier:
  KEYWORD_INLINE {
	TRACE("function-specifier", "inline");

	$function_specifier = ast_function_specifier_init(
		AST_FUNCTION_SPECIFIER_INLINE,
		&@KEYWORD_INLINE);
	if (!$function_specifier) YYNOMEM;
}
| KEYWORD__NORETURN {
	TRACE("function-specifier", "_Noreturn");

	$function_specifier = ast_function_specifier_init(
		AST_FUNCTION_SPECIFIER__NORETURN,
		&@KEYWORD__NORETURN);
	if (!$function_specifier) YYNOMEM;
}


// 6.7.5
alignment_specifier:
  KEYWORD__ALIGNAS PUNCTUATOR_LPARENTHESIS type_name[operand] PUNCTUATOR_RPARENTHESIS {
	TRACE("alignment-specifier", "_Alignas ( type-name )");

	$alignment_specifier = ast_alignas_init(
		$operand,
		&@KEYWORD__ALIGNAS,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$alignment_specifier) YYNOMEM;
}
| KEYWORD__ALIGNAS PUNCTUATOR_LPARENTHESIS constant_expression[operand] PUNCTUATOR_RPARENTHESIS {
	TRACE("alignment-specifier", "_Alignas ( constant-expression )");

	$alignment_specifier = ast_alignas_init(
		$operand,
		&@KEYWORD__ALIGNAS,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!$alignment_specifier) YYNOMEM;
}


// 6.7.6
declarator:
  direct_declarator {
	TRACE("declarator", "direct-declarator");
	$declarator = $direct_declarator;
}
| pointer direct_declarator {
	TRACE("declarator", "pointer direct-declarator");

	$declarator = $direct_declarator;

	ast_t *type = GET_CURRENT_TYPE;

	switch (*type) {
		case AST_ARRAY:
			ast_array_prepend_pointer(type, $pointer);
			break;

		case AST_FUNCTION:
			ast_function_prepend_pointer(type, $pointer);
			break;

		case AST_TYPE:
			ast_pointer_append($pointer, type);
			SET_CURRENT_TYPE($pointer);
			break;

		default:
			break;
	}
}
;


// 6.7.6
direct_declarator:
  identifier {
	TRACE("direct-declarator", "identifier");
	$direct_declarator = $identifier;
}
/*
| PUNCTUATOR_LPARENTHESIS declarator PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-declarator", "( declarator )");
	// TODO: handle pointer precedence
}
*/
| direct_declarator[array] PUNCTUATOR_LBRACKET PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ type-qualifier-list ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_declarator[array] PUNCTUATOR_LBRACKET assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ type-qualifier-list assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
/*
| direct_declarator PUNCTUATOR_LBRACKET KEYWORD_STATIC assignment-expression PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ static assignment-expression ]");
}
| direct_declarator PUNCTUATOR_LBRACKET KEYWORD_STATIC type-qualifier-list assignment-expression PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ static type-qualifier-list assignment-expression ]");
}
| direct_declarator PUNCTUATOR_LBRACKET type-qualifier-list KEYWORD_STATIC assignment-expression PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ type-qualifier-list static assignment-expression ]");
}
*/
| direct_declarator[array] PUNCTUATOR_LBRACKET PUNCTUATOR_ASTERISK PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ * ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list PUNCTUATOR_ASTERISK PUNCTUATOR_RBRACKET {
	TRACE("direct-declarator", "direct-declarator [ type-qualifier-list * ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_declarator[function] PUNCTUATOR_LPARENTHESIS function_scope_push parameter_type_list scope_pop PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-declarator", "direct-declarator ( parameter-type-list )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		$parameter_type_list,
		NULL,
		GET_VARIADIC_PARAMETER,
		&@function,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$$ = $function;
}
| direct_declarator[function] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-declarator", "direct-declarator ( )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		false,
		&@function,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$$ = $function;
}
| direct_declarator[function] PUNCTUATOR_LPARENTHESIS identifier_list PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-declarator", "direct-declarator ( identifier-list )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		NULL,
		$identifier_list,
		false,
		&@function,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$$ = $function;
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

	$$ = ast_pointer_init($child, NULL, &@PUNCTUATOR_ASTERISK);
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


// 6.7.6
parameter_type_list:
  parameter_list {
	TRACE("parameter-type-list", "parameter-list");
	$parameter_type_list = $parameter_list;
	SET_VARIADIC_PARAMETER(false);
}
| parameter_list PUNCTUATOR_COMMA PUNCTUATOR_VARIADIC {
	TRACE("parameter-type-list", "parameter-list , ...");
	$parameter_type_list = $parameter_list;
	SET_VARIADIC_PARAMETER(true);
}
;


// 6.7.6
parameter_list:
  parameter_declaration {
	TRACE("parameter-list", "parameter-declaration");

	$parameter_list = ast_declaration_list_init(
		NULL,
		$parameter_declaration,
		&@parameter_declaration,
		&@parameter_declaration);
	if (!$parameter_list) YYNOMEM;
}
| parameter_list[list] PUNCTUATOR_COMMA parameter_declaration {
	TRACE("parameter-list", "parameter-list , parameter-declaration");

	$$ = ast_declaration_list_append(
		$list,
		$parameter_declaration,
		&@parameter_declaration);
	if (!$$) YYNOMEM;
}
;


// 6.7.6
parameter_declaration:
  declaration_specifiers declarator {
	TRACE("parameter-declaration", "declaration-specifiers declarator");

	ast_t *type = GET_CURRENT_TYPE;

	if (symbol_insert_identifier(
		translation_unit->symbol_table->context.current.identifier,
		$declarator,
		type)) YYNOMEM;

	$parameter_declaration = ast_declaration_init(
		type,
		$declarator,
		NULL,
		GET_CURRENT_STORAGE_CLASS,
		&@declarator,
		&@declarator);
	if (!$parameter_declaration) YYNOMEM;

	APPEND_BASE_TYPE($declaration_specifiers);
}
| declaration_specifiers {
	TRACE("parameter-declaration", "declaration-specifiers abstract-declarator");

	ast_t *type = GET_CURRENT_TYPE;

	$parameter_declaration = ast_declaration_init(
		type,
		NULL,
		NULL,
		GET_CURRENT_STORAGE_CLASS,
		&@declaration_specifiers,
		&@declaration_specifiers);
	if (!$parameter_declaration) YYNOMEM;

	APPEND_BASE_TYPE($declaration_specifiers);
}
| declaration_specifiers abstract_declarator {
	TRACE("parameter-declaration", "declaration-specifiers abstract-declarator");

	ast_t *type = GET_CURRENT_TYPE;

	$parameter_declaration = ast_declaration_init(
		type,
		NULL,
		NULL,
		GET_CURRENT_STORAGE_CLASS,
		&@declaration_specifiers,
		&@abstract_declarator);
	if (!$parameter_declaration) YYNOMEM;

	APPEND_BASE_TYPE($declaration_specifiers);

	// handled by abstract_declarator
	(void) $abstract_declarator;
}


// 6.7.6
identifier_list:
  identifier {
	TRACE("identifier-list", "identifier");

	$identifier_list = ast_identifier_list_init($identifier, &@identifier);
	if (!$identifier_list) YYNOMEM;
}
| identifier_list[list] PUNCTUATOR_COMMA identifier {
	TRACE("identifier-list", "identifier-list , identifier");

	$$ = ast_identifier_list_append($list, $identifier, &@identifier);
	if (!$$) YYNOMEM;
}
;


// 6.7.7
type_name:
  specifier_qualifier_list {
	TRACE("type-name", "specifier-qualifier-list");
	$type_name = $specifier_qualifier_list;
}
| specifier_qualifier_list set_base_type abstract_declarator {
	TRACE("type-name", "specifier-qualifier-list abstract-declarator");
	$type_name = $abstract_declarator;

	// handled by set_base_type
	(void) $specifier_qualifier_list;
}


// 6.7.2.4
atomic_type_specifier: KEYWORD__ATOMIC PUNCTUATOR_LPARENTHESIS type_name[operand] PUNCTUATOR_RPARENTHESIS {
	TRACE("atomic-type-specifier", "_Atomic ( type-name )");

	ERROR_RESET;

	$atomic_type_specifier = ast_atomic_init(
		$operand,
		&@KEYWORD__ATOMIC,
		&@PUNCTUATOR_RPARENTHESIS,
		ERROR_ADDR);
	ERROR($atomic_type_specifier);
}


// 6.7.7
abstract_declarator:
  pointer {
	TRACE("abstract-declarator", "pointer");
	$abstract_declarator = $pointer;

	ast_pointer_append($pointer, GET_CURRENT_TYPE);
	SET_CURRENT_TYPE($pointer);
}
| direct_abstract_declarator {
	TRACE("abstract-declarator", "direct-abstract-declarator");
	$abstract_declarator = $direct_abstract_declarator;
}
| pointer direct_abstract_declarator {
	TRACE("abstract-declarator", "pointer direct-abstract-declarator");

	$abstract_declarator = $direct_abstract_declarator;

	ast_t *type = GET_CURRENT_TYPE;

	switch (*type) {
		case AST_ARRAY:
			ast_array_prepend_pointer(type, $pointer);
			break;

		case AST_FUNCTION:
			ast_function_prepend_pointer(type, $pointer);
			break;

		case AST_TYPE:
			ast_pointer_append($pointer, type);
			SET_CURRENT_TYPE($pointer);
			break;

		default:
			break;
	}
}


// 6.7.7
direct_abstract_declarator:
/*
  PUNCTUATOR_LPARENTHESIS abstract_declarator PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-abstract-declarator", "( abstract-declarator )");
	// TODO: handle pointer precedence
}
*/
  PUNCTUATOR_LBRACKET PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$direct_abstract_declarator = array;
}
| PUNCTUATOR_LBRACKET assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$direct_abstract_declarator = array;
}
| PUNCTUATOR_LBRACKET type_qualifier_list PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ type-qualifier-list ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$direct_abstract_declarator = array;
}
| PUNCTUATOR_LBRACKET type_qualifier_list assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ type-qualifier-list assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$direct_abstract_declarator = array;
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list assignment-expression ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		$type_qualifier_list,
		$assignment_expression,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
/*
| PUNCTUATOR_LBRACKET KEYWORD_STATIC assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ static assignment-expression ]");
}
| PUNCTUATOR_LBRACKET KEYWORD_STATIC type_qualifier_list assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ static type-qualifier-list assignment-expression ]");
}
| direct_abstract_declarator PUNCTUATOR_LBRACKET KEYWORD_STATIC assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ static assignment-expression ]");
}
| direct_abstract_declarator PUNCTUATOR_LBRACKET KEYWORD_STATIC type_qualifier_list assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ static type-qualifier-list assignment-expression ]");
}
| PUNCTUATOR_LBRACKET type_qualifier_list KEYWORD_STATIC assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ type-qualifier-list static assignment-expression ]");
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET type_qualifier_list KEYWORD_STATIC assignment_expression PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list static assignment-expression ]");
}
*/
| PUNCTUATOR_LBRACKET PUNCTUATOR_ASTERISK PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "[ * ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$direct_abstract_declarator = array;
}
| direct_abstract_declarator[array] PUNCTUATOR_LBRACKET PUNCTUATOR_ASTERISK PUNCTUATOR_RBRACKET {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator [ * ]");

	ast_t *array = ast_array_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		&@PUNCTUATOR_LBRACKET,
		&@PUNCTUATOR_RBRACKET);
	if (!array) YYNOMEM;

	SET_CURRENT_TYPE(array);

	$$ = $array;
}
| PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-abstract-declarator", "( )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		false,
		&@PUNCTUATOR_LPARENTHESIS,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$direct_abstract_declarator = function;
}
| PUNCTUATOR_LPARENTHESIS parameter_type_list PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-abstract-declarator", "( parameter-type-list )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		$parameter_type_list,
		NULL,
		GET_VARIADIC_PARAMETER,
		&@PUNCTUATOR_LPARENTHESIS,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$direct_abstract_declarator = function;
}
| direct_abstract_declarator[function] PUNCTUATOR_LPARENTHESIS PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator ( )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		NULL,
		NULL,
		false,
		&@function,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$$ = $function;
}
| direct_abstract_declarator[function] PUNCTUATOR_LPARENTHESIS parameter_type_list PUNCTUATOR_RPARENTHESIS {
	TRACE("direct-abstract-declarator", "direct-abstract-declarator ( parameter-type-list )");

	ast_t *function = ast_function_init(
		GET_CURRENT_TYPE,
		$parameter_type_list,
		NULL,
		GET_VARIADIC_PARAMETER,
		&@function,
		&@PUNCTUATOR_RPARENTHESIS);
	if (!function) YYNOMEM;

	SET_CURRENT_TYPE(function);

	$$ = $function;
}


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


/* mid-rule actions */


function_scope_push: %empty {
	if (scope_push(translation_unit->symbol_table)) YYNOMEM;

	translation_unit->symbol_table->context.base.storage_class =
		AST_DECLARATION_ARGUMENT;
}


scope_pop: %empty {
	 SCOPE_POP;
}


set_base_type: %empty {
	SET_BASE_TYPE($<ast>0);
}


struct_install_tag: %empty {
	uint_fast8_t  struct_or_union = $<val>-1;
	ast_t        *identifier      = $<ast>0;

	location_t *struct_or_union_yylloc = &@-1;
	location_t *identifier_yylloc      = &@0;

	STRUCT_INSTALL_TAG(
		$struct_install_tag,
		struct_or_union,
		identifier,
		struct_or_union_yylloc,
		identifier_yylloc);
}


struct_scope_push: %empty {
	if (scope_push_identifier(translation_unit->symbol_table)) YYNOMEM;
}
