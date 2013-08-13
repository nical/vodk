
Import('build_target')
Import('widget')
print ' -- ' + build_target

cpp_flags = ' -std=c++0x -g '
gtk_cpp_flags = ' `pkg-config gtkmm-3.0 clutter-gtkmm-1.0 --cflags` '
gtk_link_flags = ' `pkg-config gtkmm-3.0 clutter-gtkmm-1.0 --libs` '

include_dir = [Dir('../../src')]
libs = ['png']

gl3_libs    = ['GL', 'GLEW']
gles2_libs  = ['GLES2']
sdl_libs    = ['SDL']

engine_main = [File('src/engine/main.cpp')]
vectorizer_gtk_main = [File('src/tools/gtk/vectorizer_main.cpp')]
clutter_test_main = [File('src/tools/gtk/clutter-test.cpp')]
vodk_src =  Glob('src/vodk/*.cpp') + Glob('src/vodk/*/*.cpp') + Glob('src/kiwi/*.cpp')
gtk_tools_src = Glob('src/tools/gtk/uttils/*.cpp');

env = Environment(CPPFLAGS=cpp_flags, CPPPATH=include_dir)

if widget == 'SDL' or widget == 'Emscripten':
    env.Append(CPPDEFINES=['VODK_WIDGET_SDL'])
    libs += sdl_libs

if widget == 'Emscripten':
    env.Replace(CC = "emcc")
    env.Replace(CXX = "/home/nico/bin/em++")
    env.Replace(CPPPATH = include_dir + [Dir('/usr/include')])

if widget != 'Emscripten':
	env.Append(CPPFLAGS = gtk_cpp_flags)
	env.Append(LINKFLAGS = gtk_link_flags)
	env.Program('vectorizer-gtk', vodk_src + gtk_tools_src + vectorizer_gtk_main)
	env.Program('clutter-test', vodk_src + gtk_tools_src + clutter_test_main)

#env.Append(LIBPATH = ['/usr/local/lib/'])

libs += gl3_libs
env.Append(LIBS = libs)

p = env.Program('engine', vodk_src + engine_main)
