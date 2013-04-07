
Import('build_target')
Import('widget')
print ' -- ' + build_target

cpp_flags = '-std=c++0x -g'

include_dir = Dir('../../src')
libs = ['png']

sfml_libs   = ['sfml-graphics', 'sfml-window', 'sfml-system', 'jpeg']
gl3_libs    = ['GL', 'GLEW']
gles2_libs  = ['GLES2']
sdl_libs    = ['SDL']


vodk_src =  Glob('src/vodk/*.cpp') + Glob('src/vodk/*/*.cpp') + Glob('src/kiwi/*.cpp')

env = Environment(CPPFLAGS=cpp_flags, CPPPATH=include_dir)

if widget == 'SFML':
    env.Append(CPPDEFINES=['VODK_WIDGET_SFML'])
    libs += sfml_libs

if widget == 'SDL' or widget == 'Emscripten':
    env.Append(CPPDEFINES=['VODK_WIDGET_SDL'])
    libs += sdl_libs

#env.Append(LIBPATH = ['/usr/local/lib/'])

libs += gl3_libs
env.Append(LIBS = libs)

p = env.Program('engine', vodk_src)

if widget == 'SFML':
    Default(p)
