import os

env = Environment(CCFLAGS=['-std=c++11', '-O3', '-DNDEBUG',
                           '-march=native', '-mtune=native',
                           '-Wall', '-Wextra', '-Werror'])

for v in ['CC', 'CXX']:
    if v in os.environ:
        env[v] = os.environ[v]

if 'CPATH' in os.environ:
    env.Append(CPPPATH=os.environ['CPATH'].split(':'))

if 'LIBRARY_PATH' in os.environ:
    env.Append(LIBPATH=os.environ['LIBRARY_PATH'].split(':'))

if 'PKG_CONFIG_PATH' in os.environ:
    env.Append(ENV={'PKG_CONFIG_PATH':os.environ['PKG_CONFIG_PATH']})

env.ParseConfig('pkg-config --cflags --libs eigen3 cairomm-1.0')

env.Program('build/bin/main',
            source=['src/main.cpp',
                    'src/riemann_sphere.cpp',
                    'src/mobius.cpp',
                    'src/apollonian.cpp',
                    'src/circle.cpp',
                    'src/color.cpp',
                    'src/render.cpp'])
