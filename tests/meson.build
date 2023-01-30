# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

tests = {
        'lexer' : {
                'sources' : [
                        gen_lexer,
                ],
        },
}


if not get_option('tests').disabled() and cmocka_dep.found()
        foreach name, args : tests
                exe = executable(
                        name.underscorify(),
                        dependencies : cmocka_dep,
                        sources      : [
                                name + '.c',
                                args.get('sources', [ ]),
                        ]
                )

                test(name, exe)
        endforeach
endif
