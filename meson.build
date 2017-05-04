project('apollonian', 'cpp')
cairodep = dependency('cairomm-1.0')
sources = [
  'src/main.cpp',
  'src/mobius.cpp',
  'src/apollonian.cpp',
  'src/color.cpp',
  'src/render.cpp',
  'src/graphics.cpp'
]
executable('bin/main',
    sources: sources,
    dependencies: [cairodep],
    cpp_args: ['-std=c++14'])
