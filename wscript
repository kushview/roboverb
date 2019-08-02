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

import sys
sys.path.insert (0, "tools/waf")

import juce

def options (opt):
    opt.load ('compiler_c compiler_cxx juce')

def configure (conf):
    conf.load ('compiler_c compiler_cxx juce')
    conf.check_cxx_version ('c++14', True)
    conf.check_cfg (package = 'kv_debug-0' if conf.options.debug else 'kv-0', 
                    uselib_store='KV', args=['--libs', '--cflags'], mandatory=True)

def build (bld):
    env = bld.env.derive()
    env.cxxshlib_PATTERN = env.plugin_PATTERN
    
    bld.shlib (
        source      = bld.path.ant_glob ('roboverb/Source/*.cpp') + 
                      [ 'lv2/plugin.cpp', 'roboverb/JuceLibraryCode/BinaryData.cpp' ],
        includes    = [ 'roboverb/Source', 'lv2' ],
        use         = [ 'KV' ],
        cxxflags    = [ '-Wno-deprecated-declarations' ],
        name        = 'roboverb',
        target      = 'roboverb.lv2/roboverb',
        env         = env
    )

    bld (
        features    = 'subst',
        source      = 'lv2/manifest.ttl.in',
        target      = 'roboverb.lv2/manifest.ttl',
        LIB_EXT     = env.plugin_EXT
    )

    bld (
        features    = 'subst',
        source      = 'lv2/roboverb.ttl',
        target      = 'roboverb.lv2/roboverb.ttl'
    )
