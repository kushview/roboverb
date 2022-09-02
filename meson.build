project ('roboverb', ['c', 'cpp'], 
    version : '2.0.0',
    default_options : [ 'cpp_std=c++17' ])

lvtk_dep = dependency ('lvtk-3', 
    fallback : [ 'lvtk', 'lvtk_dep' ],
    version : '>= 2.0.0',
    default_options : [],
    required : true
)

lvtk_gl_dep = dependency ('lvtk-gl-2', 
    fallback : [ 'lvtk', 'lvtk_gl_dep' ],
    version : '>= 2.0.0',
    default_options : [],
    required : true
)

subdir ('roboverb.lv2')