// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * parser.y -- rudimentary parser
 * Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>
 */

%{
void yyerror(char const *token)
{
	(void) token;
}
%}

%%

todo: // TODO: add rules
