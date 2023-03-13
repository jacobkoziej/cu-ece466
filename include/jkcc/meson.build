# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023  Jacob Koziej <jacobkoziej@gmail.com>

jkcc_config_h = configure_file(
        input : 'config.h.in',
        output : 'config.h',
        configuration : {
                'JKCC_CONFIG_OPTION_TRACE' : get_option('trace'),
        },
)
