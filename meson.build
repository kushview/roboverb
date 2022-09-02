project ('roboverb', ['c', 'cpp'], 
    version : '2.0.0',
    default_options : [ 'cpp_std=c++17' ])

pugl_gl_dep = dependency ('pugl-gl-0', 
    fallback : [ 'pugl', 'gl_backend_dep' ],
    version : '>= 0.4.0',
    default_options : [
        'tests=disabled',
        'examples=disabled',
        'cairo=disabled',
        'vulkan=disabled',
        'stub=false',
        'default_library=static'
    ],
    required : true
)

lvtk_dep = dependency ('lvtk-3', 
    fallback : [ 'lvtk', 'lvtk_dep' ],
    version : '>= 2.0.0',
    default_options : [],
    required : true
)

subdir ('roboverb.lv2')