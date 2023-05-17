// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * util.c -- x86 target code generation utils
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <jkcc/target/x86/util.h>

#include <stddef.h>
#include <stdint.h>

#include <jkcc/ast.h>
#include <jkcc/constant.h>


int target_x86_util_ebp_offset(uintptr_t reg, uintptr_t args)
{
	if (reg < args) return (reg * 4) + 8;

	return -((reg - args) * 4) - 4;
}

size_t target_x86_util_sizeof(ast_t *type)
{
	// TODO: FIX THIS CRIME!
	// we're assuming everything's a 4-byte int
	size_t size = 4;

	while (*type != AST_TYPE) {
		switch (*type) {
			case AST_ARRAY: {
				ast_t *ast_size = ast_array_get_size(type);

				if (*ast_size != AST_INTEGER_CONSTANT)
					return 0;

				const integer_constant_t *integer_constant
					= ast_integer_constant_get_integer_constant(ast_size);

				switch (integer_constant->type) {
					case INT:
						size *= integer_constant->INT;
						break;

					case UNSIGNED_INT:
						size *= integer_constant->UNSIGNED_INT;
						break;

					case LONG_INT:
						size *= integer_constant->LONG_INT;
						break;

					case UNSIGNED_LONG_INT:
						size *= integer_constant->UNSIGNED_LONG_INT;
						break;

					case LONG_LONG_INT:
						size *= integer_constant->LONG_LONG_INT;
						break;

					case UNSIGNED_LONG_LONG_INT:
						size *= integer_constant->UNSIGNED_LONG_LONG_INT;
						break;
				}

				type = ast_array_get_type(type);

				break;
			}

			case AST_POINTER:
				type = ast_pointer_get_pointer(type);
				break;

			case AST_TYPE:
				break;

			default:
				return 0;
		}
	}

	return size;
}
