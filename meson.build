project ('roboverb', ['c', 'cpp'], 
    version : '2.0.0',
    default_options : [ 
        'cpp_std=c++17', 
        'default_library=static',
        'b_ndebug=if-release',
        'buildtype=release',
        'c_std=c99',
    ])

if not meson.is_subproject()
    add_global_arguments (['-DLVTK_STATIC=1', '-DPUGL_STATIC=1'], 
        language : [ 'c', 'cpp' ])
endif

lvtk_opts = [
    'doc=disabled',
    'test=disabled',
    'demo=enabled',
    'default_library=static'
]

lvtk_dep = dependency ('lvtk-3', 
    fallback : [ 'lvtk', 'lvtk_dep' ],
    version : '>= 3.0.0',
    default_options : lvtk_opts,
    required : true
)

lvtk_cairo_dep = dependency ('lui-cairo-0', 
    fallback : [ 'lui', 'lui_cairo_dep' ],
    version : '>= 0.0.1',
    default_options : lvtk_opts,
    required : true
)

subdir ('roboverb.lv2')
