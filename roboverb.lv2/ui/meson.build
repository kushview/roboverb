roboverb_ui_sources = '''
    nanovg/nanovg.c
    nanovg/nanovg_gl.c
'''.split()

roboverb_ui_lib = static_library ('roboverb-ui',
    roboverb_ui_sources,
    c_args : [ '-DNANOVG_GL3=1', '-Wno-implicit-function-declaration',
               '-DPUGL_DISABLE_DEPRECATED' ],
    gnu_symbol_visibility : 'hidden',
    include_directories : [ 'nanovg' ],
    dependencies : [ pugl_gl_dep ])

roboverb_ui_dep = declare_dependency (
    link_with : roboverb_ui_lib,
    include_directories : [ '.', 'nanovg' ])
