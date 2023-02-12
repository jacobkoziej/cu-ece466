# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

tests = {
        'lexer' : {
                'args' : [
                        files(
                                'lexer.d/identifiers',
                                'lexer.d/keywords',
                                'lexer.d/punctuators',
                        ),
                ],
                'sources' : [
                        lex_yy_c,
                        parser_tab_c,
                ],
        },
}


if not get_option('tests').disabled() and cmocka_dep.found()
        foreach name, args : tests
                exe = executable(
                        name.underscorify(),
                        dependencies        : cmocka_dep,
                        include_directories : jkcc_inc,
                        sources      : [
                                name + '.c',
                                args.get('sources', [ ]),
                        ]
                )

                test(
                        name,
                        exe,
                        args : args.get('args', [ ]),
                )
        endforeach
endif
