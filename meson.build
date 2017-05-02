project('apollonian', 'cpp')
cairodep = dependency('cairomm-1.0')
sources = [
  'src/main.cpp',
  'src/mobius.cpp',
  'src/apollonian.cpp',
  'src/color.cpp',
  'src/render.cpp',
]
executable('bin/main',
    sources: sources,
    dependencies: [cairodep],
    cpp_args: ['-std=c++14'])
test_sources = [
  'src/color.cpp',
  'src/graphics.cpp',
  'src/test_image_buffer.cpp'
]
executable('bin/test_image_buffer',
    sources: test_sources,
    dependencies: [cairodep])
