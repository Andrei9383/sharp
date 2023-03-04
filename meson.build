project('sharp', 'cpp',
  version : '0.1',
  default_options : ['warning_level=0'])

sdl2_dep = dependency('sdl2')
exe = executable('sharp', 'sharp.cpp',
  install : true, dependencies : sdl2_dep)

test('basic', exe)
