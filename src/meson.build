
fs = import ('fs')

roboverb_sources = files ('''
    plugin.cpp
    roboverb.cpp
'''.split())

roboverb_ui_type = 'X11UI'
if host_machine.system() == 'windows'
    roboverb_ui_type = 'WindowsUI'
elif host_machine.system() == 'darwin'
    roboverb_ui_type = 'CocoaUI'
endif

plugin_install_dir = get_option('lv2dir').strip()
if plugin_install_dir != ''
    plugin_install_dir = fs.expanduser (plugin_install_dir)
endif

if plugin_install_dir == ''
    if target_machine.system() == 'darwin'
        plugin_install_dir = fs.expanduser ('~/Library/Audio/Plug-Ins/LV2')
    elif target_machine.system() == 'haiku'
        plugin_install_dir = '/boot/common/add-ons/lv2'
    elif target_machine.system() == 'windows'
        plugin_install_dir = fs.expanduser ('~/AppData/Roaming/LV2')
    else
        plugin_install_dir = fs.expanduser('~/.lv2')
    endif
endif

plugin_install_dir = plugin_install_dir / 'roboverb.lv2'

plugin = shared_module ('roboverb',
    roboverb_sources,
    name_prefix : '',
    dependencies : [ lvtk_dep ],
    install : true,
    install_dir : plugin_install_dir,
    gnu_symbol_visibility : 'hidden'
)

roboverb_ui_sources = files('''
    lv2ui.cpp
    res.cpp
'''.split())
ui = shared_module ('roboverb-ui',
    roboverb_ui_sources,
    name_prefix : '',
    dependencies : [ lvtk_dep, lui_cairo_dep, clap_dep ],
    include_directories : [ '.' ],
    install : true,
    install_dir : plugin_install_dir,
    cpp_args : [ ],
    gnu_symbol_visibility : 'hidden'
)

manifest_ttl = configure_file (
    input : 'manifest.ttl.in',
    output : 'manifest.ttl',
    configuration : {
        'BINARY' : fs.name (plugin.full_path()),
        'UI_BINARY' : fs.name (ui.full_path()),
        'UI_TYPE' : roboverb_ui_type
    },
    install : true,
    install_dir : plugin_install_dir
)

roboverb_ttl = configure_file (
    input : 'roboverb.ttl.in',
    output : 'roboverb.ttl',
    copy : true,
    install : true,
    install_dir : plugin_install_dir
)

lv2_validate = find_program ('lv2_validate', required : false)
if lv2_validate.found()
    test ('lv2_validate', lv2_validate, args : [ manifest_ttl, roboverb_ttl ])
endif

summary ('Install', plugin_install_dir, section : 'LV2')

# Clap
clap_install_dir = get_option ('clapdir').strip()
if clap_install_dir != ''
    clap_install_dir = fs.expanduser (clap_install_dir)
endif

if clap_install_dir == ''
    if target_machine.system() == 'darwin'
        clap_install_dir = fs.expanduser ('~/Library/Audio/Plug-Ins/CLAP')
    elif target_machine.system() == 'haiku'
        clap_install_dir = '/boot/common/add-ons/clap'
    elif target_machine.system() == 'windows'
        clap_install_dir = fs.expanduser ('~/AppData/Roaming/CLAP')
    else
        clap_install_dir = fs.expanduser('~/.clap')
    endif
endif

clap_plugin = shared_module ('roboverb',
    [ roboverb_sources, 'res.cpp', 'clap.cpp' ],
    name_prefix : '',
    name_suffix : 'clap',
    include_directories: [ ],
    dependencies : [ lvtk_dep, clap_dep, clap_helpers_dep, lui_cairo_dep ],
    install : true,
    install_dir : clap_install_dir,
    link_args : [ ],
    gnu_symbol_visibility : 'hidden'
)

summary ('Install', clap_install_dir, section : 'CLAP')
