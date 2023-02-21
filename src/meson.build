# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

y_tab_c = custom_target(
        'y.tab.c',
        input  : 'parser.y',
        output : ['y.tab.c', 'y.tab.h'],
        command : [
                bison,
                '--header=@OUTPUT1@',
                '--output=@OUTPUT0@',
                '@INPUT0@',
        ],
)


lex_yy_c = custom_target(
        'lex.yy.c',
        input   : ['lexer.l', y_tab_c],
        output  : ['lex.yy.c', 'lex.yy.h'],
        command : [
                flex,
                '--outfile=@OUTPUT0@',
                '--header-file=@OUTPUT1@',
                '-Cef',
                '@INPUT0@',
        ],
)


jkcc_src = files(
        'ast.c',
        'lexer.c',
        'string.c',
)


executable(
        'jkcc',
        include_directories : jkcc_inc,
        sources : [
                'jkcc.c',
                jkcc_src,
                lex_yy_c,
                y_tab_c,
        ],
)
