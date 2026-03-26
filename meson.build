project(
    'SemiTOV-assembler',
    'c',
    version: '0.1',
    default_options: ['warning_level=3', 'c_std=c11'],
)

sources = []
subdir('src')
subdir('gperf')

inc = include_directories('include')

executable(
    'assembler',
    sources,
    include_directories: inc,
)
