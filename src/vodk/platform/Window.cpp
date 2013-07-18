
#include "vodk/platform/Window.hpp"

#ifdef VODK_WIDGET_SDL
#include "GL/glew.h"
#include <stdio.h>
#include "SDL/SDL.h"
#endif // VODK_WIDGET_SDL

namespace vodk {
namespace gpu {
class RenderingContext;

bool initBackend();
RenderingContext* createRenderingContext(io::Window* w);
void deleteRenderingContext(RenderingContext* ctx);

} // namespace

namespace io {

#ifdef VODK_WIDGET_SDL

Window::Window(int w, int h, const char* title)
{
    SDL_Surface *screen;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    screen = SDL_SetVideoMode(w, h, 24, SDL_OPENGL);
    if (!screen) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return;
    }
    gpu::initBackend();
    _gl = gpu::createRenderingContext(this);
}

Window::~Window() {
    gpu::deleteRenderingContext(_gl);
}

gpu::RenderingContext* Window::getRenderingContext() {
    return _gl;
}

void Window::display()
{
    SDL_GL_SwapBuffers();
#if !EMSCRIPTEN
    SDL_Delay(16);
#endif
}

bool Window::pollEvent(Event& e)
{
    SDL_Event sdl_evt;
    bool ret = SDL_PollEvent(&sdl_evt);
    switch (sdl_evt.type) {
        case SDL_KEYDOWN: e.type = Event::KeyDown; break;
        case SDL_QUIT:    e.type = Event::Closed; break;
    }

    return ret;
}

#endif // VODK_WIDGET_SDL


} // namespace
} // namespace
