// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * list.c -- list basic block
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/ir/bb/list.h>
#include <jkcc/ir/ir.h>
#include <jkcc/private/ir.h>

#include <jkcc/ast.h>
#include <jkcc/ir.h>
#include <jkcc/vector.h>


int ir_bb_list_gen(
	ir_context_t *ir_context,
	ast_t        *ast)
{
	vector_t *list = ast_list_get_list(ast);

	ast_t **statement = list->buf;
	for (size_t i = 0; i < list->use; i++) {
		int ret = IR_BB_GEN(ir_context, statement[i]);
		if (ret) return ret;
	}

	return 0;
}
