# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

y_tab_c = custom_target(
        'y.tab.c',
        input  : 'parser.y',
        output : ['y.tab.c', 'y.tab.h', 'y.output'],
        command : [
                bison,
                '--color=yes',
                '--header=@OUTPUT1@',
                '--output=@OUTPUT0@',
                '--report-file=@OUTPUT2@',
                '--report=all',
                '--warnings=all',
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
        'jkcc.c',
        'lexer.c',
        'parser.c',
        'string.c',
        'trace.c',
        'vector.c',
)


subdir('ast')


executable(
        'jkcc',
        include_directories : jkcc_inc,
        sources : [
                'main.c',
                jkcc_src,
                lex_yy_c,
                version_h,
                y_tab_c,
        ],
)
