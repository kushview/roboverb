#!/usr/bin/evn python
# encoding: utf-8
# Copyright (C) 2012-2016 Michael Fisher <mfisher@kushview.net>

''' This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public Licence as published by
the Free Software Foundation, either version 2 of the Licence, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
file COPYING for more details. '''

import sys, os
from os.path import expanduser
sys.path.insert (0, "tools/waf")
import juce

APPNAME = 'roboverb'
VERSION = '1.1.0'
top = '.'
out = 'build'

def options (opt):
    opt.load ('compiler_c compiler_cxx juce')
    opt.add_option ('--lv2-path', default='', type='string', \
        dest='lv2_path', help="Specifiy a custom path to install the LV2 bundle")
    opt.add_option ('--lv2-user', default=False, action='store_true', \
        dest='lv2_user', help="Install to LV2 user path")

def configure (conf):
    conf.load ('compiler_c compiler_cxx juce')
    conf.check_cxx_version ('c++17', True)

    conf.check_cfg (package = 'juce_audio_basics-5' if not conf.options.debug else 'juce_audio_basics_debug-5', 
                    uselib_store='JUCE_AUDIO_BASICS', 
                    args=['juce_audio_basics-5 >= 5.4.5', '--libs', '--cflags'], 
                    mandatory=True)
    conf.check_cfg (package = 'juce_gui_basics-5' if not conf.options.debug else 'juce_gui_basics_debug-5', 
                    uselib_store='JUCE_GUI_BASICS', 
                    args=['juce_gui_basics-5 >= 5.4.5', '--libs', '--cflags'], mandatory=True)
    
    conf.check_cfg (package = 'lvtk-2', uselib_store='LVTK', args=['--cflags'], mandatory=True)

    if len (conf.options.lv2_path) > 0:
        conf.env.BUNDLEDIR = os.path.join (conf.options.lv2_path, 'roboverb.lv2')

    else:
        if juce.is_linux():
            if conf.options.lv2_user:
                conf.env.BUNDLEDIR = os.path.join (expanduser("~"), '.local/lib/lv2/roboverb.lv2')
            else:
                conf.env.BUNDLEDIR = conf.env.PREFIX + '/lib/lv2/roboverb.lv2'
        
        elif juce.is_mac():
            if conf.options.lv2_user:
                conf.env.BUNDLEDIR = os.path.join (expanduser("~"), 'Library/Audio/Plug-Ins/LV2/roboverb.lv2')
            else:
                conf.env.BUNDLEDIR = '/Library/Audio/Plug-Ins/LV2/roboverb.lv2'

        else:
            conf.env.BUNDLEDIR = conf.env.PREFIX + '/lib/lv2/roboverb.lv2'

    juce.display_header ("Roboverb")
    juce.display_msg (conf, "LV2 Bundle", conf.env.BUNDLEDIR)

def build (bld):
    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    
    bld.shlib (
        source          = bld.path.ant_glob ('roboverb.lv2/*.cpp'),
        includes        = [ 'roboverb.lv2', 'roboverb.lv2/compat' ],
        use             = [ 'LVTK' ],
        cxxflags        = [ '-Wno-deprecated-declarations' ],
        name            = 'roboverb',
        target          = 'roboverb.lv2/roboverb',
        env             = env,
        install_path    = bld.env.BUNDLEDIR
    )

    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    bld.shlib (
        source          = bld.path.ant_glob ('roboverb.lv2/ui/*.cpp') + [ 'roboverb.lv2/Roboverb.cpp' ],
        includes        = [ 'roboverb.lv2', 'roboverb.lv2/compat' ],
        use             = [ 'JUCE_GUI_BASICS', 'LVTK' ],
        cxxflags        = [ '-Wno-deprecated-declarations', '-DROBOVERB_UI', '-DROBOVERB_LV2' ],
        name            = 'roboverb_ui',
        target          = 'roboverb.lv2/roboverb_ui',
        env             = env,
        install_path    = bld.env.BUNDLEDIR
    )

    bld (
        features    = 'subst',
        source      = 'roboverb.lv2/manifest.ttl.in',
        target      = 'roboverb.lv2/manifest.ttl',
        LIB_EXT     = env.plugin_EXT,
        install_path    = bld.env.BUNDLEDIR
    )

    bld (
        features    = 'subst',
        source      = 'roboverb.lv2/roboverb.ttl',
        target      = 'roboverb.lv2/roboverb.ttl',
        install_path = bld.env.BUNDLEDIR
    )
