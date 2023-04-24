// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast/ast.h -- ast type
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_AST_H
#define JKCC_AST_AST_H


typedef enum ast_e {
	AST_ADDRESSOF,
	AST_ALIGNAS,
	AST_ALIGNOF,
	AST_ARGUMENT_LIST,
	AST_ARRAY,
	AST_ASSIGNMENT,
	AST_ATOMIC,
	AST_BINARY_OPERATOR,
	AST_CALL,
	AST_CAST,
	AST_CHARACTER_CONSTANT,
	AST_DECLARATION,
	AST_DECLARATION_LIST,
	AST_DEREFERENCE,
	AST_EXPRESSION,
	AST_FLOATING_CONSTANT,
	AST_FUNCTION,
	AST_FUNCTION_SPECIFIER,
	AST_GENERIC_ASSOCIATION,
	AST_GENERIC_ASSOCIATION_LIST,
	AST_GENERIC_SELECTION,
	AST_IDENTIFIER,
	AST_IDENTIFIER_LIST,
	AST_IF,
	AST_INTEGER_CONSTANT,
	AST_MEMBER_ACCESS,
	AST_POINTER,
	AST_SIZEOF,
	AST_STATIC_ASSERT,
	AST_STORAGE_CLASS_SPECIFIER,
	AST_STRING_LITERAL,
	AST_STRUCT,
	AST_SWITCH,
	AST_TERNARY_OPERATOR,
	AST_TYPE,
	AST_TYPE_QUALIFIER,
	AST_TYPE_QUALIFIER_LIST,
	AST_TYPE_SPECIFIER,
	AST_UNARY_OPERATOR,
	AST_NODES_TOTAL,
} ast_t;


#endif  /* JKCC_AST_AST_H */
