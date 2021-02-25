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
    opt.add_option ('--no-juceui', default=True, action='store_false', \
        dest='juceui', help="Install to LV2 user path")

def configure (conf):
    conf.load ('compiler_c compiler_cxx juce')
    conf.check_cxx_version ('c++17', True)

    conf.env.FRAMEWORK_COCOA   = 'Cocoa'
    conf.env.FRAMEWORK_OPEN_GL = 'OpenGL'
    
    conf.check_cfg (package = 'lv2', uselib_store='LV2', args=['--cflags'], mandatory=True)

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

def build_turtle (bld):
    env = bld.env
    manifesttl = bld (
        features        = 'subst',
        source          = 'roboverb.lv2/manifest.ttl.in',
        target          = 'roboverb.lv2/manifest.ttl',
        install_path    = bld.env.BUNDLEDIR,
        LIB_EXT         = env.plugin_EXT
    )

    roboverbttl = bld (
        features        = 'subst',
        source          = 'roboverb.lv2/roboverb.ttl.in',
        target          = 'roboverb.lv2/roboverb.ttl',
        install_path    = bld.env.BUNDLEDIR
    )

def build_ui (bld):
    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    nativeui = bld.shlib (
            source  = [ 
                'roboverb.lv2/nk/UI.cpp',
                'libs/lvtk/libs/pugl/pugl/detail/implementation.c'
            ],
            includes        = [
                'roboverb.lv2', 
                'libs/lvtk', 
                'libs/lvtk/libs/pugl',
                'libs/lvtk/libs/nuklear'
            ],
            use = [ 'LV2' ],
            cflags = [ '-Wno-deprecated-declarations', '-fvisibility=hidden' ],
            cxxflags  = [ '-Wno-deprecated-declarations', '-fvisibility=hidden' ],
            name = 'native',
            target = 'roboverb.lv2/native',
            env = env,
            install_path = bld.env.BUNDLEDIR
        )

    if sys.platform == 'darwin':
        nativeui.source.append ('libs/lvtk/libs/pugl/pugl/detail/mac.m')
        nativeui.source.append ('libs/lvtk/libs/pugl/pugl/detail/mac_gl.m')
        nativeui.use.append ('COCOA')
        nativeui.use.append ('OPEN_GL')

def build (bld):
    build_turtle (bld)

    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    
    roboverb = bld.shlib (
        source          = bld.path.ant_glob ('roboverb.lv2/*.cpp'),
        includes        = [ 'roboverb.lv2', 'roboverb.lv2/compat', 'libs/lvtk' ],
        use             = [ 'LV2' ],
        cxxflags        = [ '-Wno-deprecated-declarations', '-fvisibility=hidden' ],
        name            = 'roboverb',
        target          = 'roboverb.lv2/roboverb',
        env             = env,
        install_path    = bld.env.BUNDLEDIR
    )

    # bld.add_group()
    # build_ui(bld)
