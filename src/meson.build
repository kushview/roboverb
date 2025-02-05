
# Clap
clap_install_dir = '' # get_option ('clapdir').strip()
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
    [roboverb_sources, roboverb_ui_sources, 'clap.cpp'],
    name_prefix : '',
    name_suffix : 'clap',
    include_directories: [ '../roboverb.lv2' ],
    dependencies : [ lvtk_dep, clap_dep, clap_helpers_dep, lui_cairo_dep ],
    install : true,
    install_dir : clap_install_dir,
    link_args : [ ],
    gnu_symbol_visibility : 'hidden'
)
