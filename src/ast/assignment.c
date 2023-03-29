// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * assignment.c -- assignment ast node
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ast.h>
#include <jkcc/ast/assignment.h>
#include <jkcc/private/ast.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <jkcc/lexer.h>


ast_t *ast_assignment_init(
	ast_t         *lvalue,
	ast_t         *rvalue,
	uint_fast16_t  assignment,
	location_t    *location_start,
	location_t    *location_end)
{
	AST_INIT(ast_assignment_t);

	node->lvalue     = lvalue;
	node->rvalue     = rvalue;
	node->assignment = assignment;

	node->location.file  = location_start->file;
	node->location.start = location_start->start;

	node->location.end = (location_end)
		? location_end->end
		: location_start->end;

	AST_RETURN(AST_ASSIGNMENT);
}

void ast_assignment_free(ast_t *ast)
{
	AST_FREE(ast_assignment_t);

	AST_NODE_FREE(node->lvalue);
	AST_NODE_FREE(node->rvalue);

	free(node);
}

void fprint_ast_assignment(
	FILE         *stream,
	const ast_t  *ast,
	size_t        level,
	uint_fast8_t  flags)
{
	FPRINT_AST_NODE_BEGIN(ast_assignment_t);

	FPRINT_AST_MEMBER("lvalue", node->lvalue);
	FPRINT_AST_MEMBER("rvalue", node->rvalue);

	const char *assignment;
	switch (node->assignment) {
		case AST_ASSIGNMENT_ASSIGNMENT:
			assignment = "=";
			break;

		case AST_ASSIGNMENT_COMPOUND_MULTIPLICATION:
			assignment = "*=";
			break;

		case AST_ASSIGNMENT_COMPOUND_DIVISION:
			assignment = "/=";
			break;

		case AST_ASSIGNMENT_COMPOUND_MODULO:
			assignment = "%=";
			break;

		case AST_ASSIGNMENT_COMPOUND_ADDITION:
			assignment = "+=";
			break;

		case AST_ASSIGNMENT_COMPOUND_SUBTRACTION:
			assignment = "-=";
			break;

		case AST_ASSIGNMENT_COMPOUND_LBITSHIFT:
			assignment = "<<=";
			break;

		case AST_ASSIGNMENT_COMPOUND_RBITSHIFT:
			assignment = ">>=";
			break;

		case AST_ASSIGNMENT_COMPOUND_AND:
			assignment = "&=";
			break;

		case AST_ASSIGNMENT_COMPOUND_XOR:
			assignment = "^=";
			break;

		case AST_ASSIGNMENT_COMPOUND_OR:
			assignment = "|=";
			break;

		default:
			assignment = "(unknown)";
			break;
	}

	FPRINT_AST_FIELD("assignment", assignment);

	FPRINT_AST_NODE_FINISH;
}
