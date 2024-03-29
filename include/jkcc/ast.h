// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * ast.h -- abstract syntax tree
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef JKCC_AST_H
#define JKCC_AST_H


#include <jkcc/ast/addressof.h>
#include <jkcc/ast/alignas.h>
#include <jkcc/ast/alignof.h>
#include <jkcc/ast/array.h>
#include <jkcc/ast/assignment.h>
#include <jkcc/ast/ast.h>
#include <jkcc/ast/atomic.h>
#include <jkcc/ast/binary_operator.h>
#include <jkcc/ast/break.h>
#include <jkcc/ast/call.h>
#include <jkcc/ast/case.h>
#include <jkcc/ast/cast.h>
#include <jkcc/ast/character_constant.h>
#include <jkcc/ast/continue.h>
#include <jkcc/ast/declaration.h>
#include <jkcc/ast/dereference.h>
#include <jkcc/ast/empty.h>
#include <jkcc/ast/expression.h>
#include <jkcc/ast/floating_constant.h>
#include <jkcc/ast/for.h>
#include <jkcc/ast/function.h>
#include <jkcc/ast/function_specifier.h>
#include <jkcc/ast/generic_association.h>
#include <jkcc/ast/generic_association_list.h>
#include <jkcc/ast/generic_selection.h>
#include <jkcc/ast/goto.h>
#include <jkcc/ast/identifier.h>
#include <jkcc/ast/if.h>
#include <jkcc/ast/integer_constant.h>
#include <jkcc/ast/label.h>
#include <jkcc/ast/list.h>
#include <jkcc/ast/member_access.h>
#include <jkcc/ast/pointer.h>
#include <jkcc/ast/return.h>
#include <jkcc/ast/sizeof.h>
#include <jkcc/ast/static_assert.h>
#include <jkcc/ast/storage_class_specifier.h>
#include <jkcc/ast/string_literal.h>
#include <jkcc/ast/struct.h>
#include <jkcc/ast/switch.h>
#include <jkcc/ast/ternary_operator.h>
#include <jkcc/ast/translation_unit.h>
#include <jkcc/ast/type.h>
#include <jkcc/ast/type_qualifier.h>
#include <jkcc/ast/type_specifier.h>
#include <jkcc/ast/unary_operator.h>
#include <jkcc/ast/while.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <jkcc/location.h>


#define AST_PRINT_NO_INDENT_INITIAL   (1 << 0)
#define AST_PRINT_NO_TRAILING_NEWLINE (1 << 1)

#define AST_NODE_FREE(ast) if (ast) ast_node_free[*ast](ast);

#define FPRINT_AST_NODE(stream, ast, level, flags) fprint_ast_node[*ast]( \
	stream,                                                           \
	ast,                                                              \
	level,                                                            \
	flags)

#define AST_NODE_STR(ast) ast_node_str[*ast]

#define OFFSETOF_AST_NODE(node, type) ((type*) (((uintptr_t) node) - offsetof(type, ast)))


extern void (*const ast_node_free[AST_NODES_TOTAL])(ast_t *ast);

extern void (*const fprint_ast_node[AST_NODES_TOTAL])(
	FILE         *stream,
	const ast_t  *ast,
	size_t       level,
	uint_fast8_t flags);

extern const char *const ast_node_str[AST_NODES_TOTAL];


void fprint_file(
	FILE             *stream,
	const file_t     *file,
	size_t            level,
	uint_fast8_t      flags);
void fprint_location(
	FILE             *stream,
	const location_t *location,
	size_t            level,
	uint_fast8_t      flags);


#endif  /* JKCC_AST_H */
