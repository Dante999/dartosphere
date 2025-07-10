def Settings( **kwargs ):
  return { 'flags': ['-Wall', 'Wextra', '-Wpedantic',
                     '-I/usr/include/SDL2',
                     '-Ithirdparty/libcutils/include'
  ] }

