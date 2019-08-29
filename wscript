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

    conf.env.JUCEUI = conf.options.juceui and bool(conf.env.HAVE_JUCE_GUI_BASICS)
    juce.display_header ("Roboverb")
    juce.display_msg (conf, "LV2 Bundle", conf.env.BUNDLEDIR)
    juce.display_msg (conf, "JUCEUI", conf.env.JUCEUI)

def build (bld):
    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    
    roboverb = bld.shlib (
        source          = bld.path.ant_glob ('roboverb.lv2/*.cpp'),
        includes        = [ 'roboverb.lv2', 'roboverb.lv2/compat' ],
        use             = [ 'LVTK' ],
        cxxflags        = [ '-Wno-deprecated-declarations', '-fvisibility=hidden' ],
        name            = 'roboverb',
        target          = 'roboverb.lv2/roboverb',
        env             = env,
        install_path    = bld.env.BUNDLEDIR
    )

    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    juceui = None
    if bld.env.JUCEUI:
        juceui = bld.shlib (
            source          = bld.path.ant_glob ('roboverb.lv2/ui/*.cpp') + [ 'roboverb.lv2/Roboverb.cpp' ],
            includes        = [ 'roboverb.lv2', 'roboverb.lv2/compat' ],
            use             = [ 'JUCE_GUI_BASICS', 'LVTK' ],
            cxxflags        = [ '-Wno-deprecated-declarations', '-fvisibility=hidden', 
                                '-DROBOVERB_UI', '-DROBOVERB_LV2' ],
            name            = 'juceui',
            target          = 'roboverb.lv2/juceui',
            env             = env,
            install_path    = bld.env.BUNDLEDIR
        )

    manifesttl = bld (
        features        = 'subst',
        source          = 'roboverb.lv2/manifest.ttl.in',
        target          = 'roboverb.lv2/manifest.ttl',
        install_path    = bld.env.BUNDLEDIR,
        LIB_EXT         = env.plugin_EXT,
        INCLUDE_JUCEUI  = '',
    )

    roboverbttl = bld (
        features        = 'subst',
        source          = 'roboverb.lv2/roboverb.ttl.in',
        target          = 'roboverb.lv2/roboverb.ttl',
        install_path    = bld.env.BUNDLEDIR
    )

    if bld.env.JUCEUI:
        bld (
            features        = 'subst',
            source          = 'roboverb.lv2/juceui.ttl',
            target          = 'roboverb.lv2/juceui.ttl',
            install_path    = bld.env.BUNDLEDIR,
            LIB_EXT         = env.plugin_EXT,
            INCLUDE_JUCEUI  = '',
        )
        manifesttl.INCLUDE_JUCEUI = '''<https://kushview.net/plugins/roboverb/juceui>
    a lvtk:JUCEUI ;
    ui:binary <juceui%s> ;
    rdfs:seeAlso <juceui.ttl> .''' % (env.plugin_EXT)
