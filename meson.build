project('apollonian', 'cpp')

cairodep = dependency('cairomm-1.0')
threaddep = dependency('threads')

sources = [
  'src/main.cpp',
  'src/mobius.cpp',
  'src/apollonian.cpp',
  'src/color.cpp',
  'src/render.cpp',
  'src/graphics.cpp',
  'src/visitor.cpp'
]

main_prog = executable('main',
  sources: sources,
  dependencies: [cairodep, threaddep],
  cpp_args: ['-std=c++14'])

custom_target('result',
  output: 'apollonian.png',
  command: [main_prog, '@OUTPUT@'],
  build_by_default: true,
  console: true)
