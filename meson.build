project(
    'SemiTOV-assembler',
    'c',
    version: '0.1',
    default_options: ['warning_level=3', 'c_std=c11'],
)

gperf = find_program('gperf', required: false)

if gperf.found()
    run_target(
        'gperf',
        command: [
            gperf,
            '-t',
            '-C',
            '--output-file=' + join_paths(meson.current_source_dir(), 'gperf/hash_utils.c'),
            join_paths(meson.current_source_dir(), 'gperf/test.gperf'),
        ],
    )
endif


sources = []
subdir('src')
subdir('gperf')

inc = include_directories('include')


executable(
    'assembler',
    sources,
    include_directories: inc,
)
