

SConscript('SConscript',
            variant_dir='build/sfml',
            duplicate=0,
            exports={'build_target':'debug',
                     'widget':'SFML'})

SConscript('SConscript',
            variant_dir='build/sdl',
            duplicate=0,
            exports={'build_target':'debug',
                     'widget':'SDL'})

SConscript('SConscript',
            variant_dir='build/emscripten',
            duplicate=0,
            exports={'build_target':'debug',
                     'widget':'Emscripten'})

SConscript('SConsTests',
            variant_dir='build/tests',
            duplicate=0,
            exports={'build_target':'debug',
                     'widget':'Emscripten'})
