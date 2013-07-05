#ifndef VODK_CORE_CONFIG_HPP
#define VODK_CORE_CONFIG_HPP


#ifndef EMSCRIPTEN

#if !defined(VODK_WIDGET_SDL) && !defined(VODK_WIDGET_SFML)
#define VODK_WIDGET_SFML
#endif // !SDL

#ifndef VODK_GL_ES2
#define VODK_GL_3
#endif // GL_ES2

#else // !EMSCIPTEN

#define VODK_WIDGET_SDL
#define VODK_GL_ES2

#ifndef CHAR_BIT
#define CHAR_BIT __CHAR_BIT__
#endif

#endif // EMSCRIPTEN



#endif
