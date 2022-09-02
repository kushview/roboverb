fs = import ('fs')

roboverb_sources = '''
    Plugin.cpp
    Roboverb.cpp
'''.split()

plugin_install_dir = 'lib' / 'lv2' / 'roboverb.lv2'

plugin = shared_module ('roboverb', 
    [ 'Plugin.cpp',
    'Roboverb.cpp' ],
    name_prefix : '',
    dependencies : [ lvtk_dep ],
    install : true,
    install_dir : plugin_install_dir,
    gnu_symbol_visibility : 'hidden'
)

subdir ('ui')

ui = shared_module ('roboverb-ui',
    'UI.cpp',
    name_prefix : '',
    dependencies : [ lvtk_dep, pugl_gl_dep, roboverb_ui_dep ],
    include_directories : [ '.', 'ui', 'ui/nanovg' ],
    install : true,
    install_dir : plugin_install_dir,
    cpp_args : [ '-DPUGL_DISABLE_DEPRECATED' ],
    gnu_symbol_visibility : 'hidden'
)

configure_file (
    input : 'manifest.ttl.in',
    output : 'manifest.ttl',
    configuration : {
        'BINARY' : fs.name (plugin.full_path())
    },
    install : true,
    install_dir : plugin_install_dir
)

configure_file (
    input : 'roboverb.ttl.in',
    output : 'roboverb.ttl',
    copy : true,
    install : true,
    install_dir : plugin_install_dir
)
