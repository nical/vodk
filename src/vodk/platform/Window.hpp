
#ifndef VODK_PLATFORM_WINDOW_HPP
#define VODK_PLATFORM_WINDOW_HPP

#include "vodk/core/config.hpp"

namespace vodk {

namespace gpu {
    class RenderingContext;
} // namespace

namespace io {

#ifdef VODK_WIDGET_SFML
class Window : public sf::Window {
public:
    typedef sf::Event Event;

    Window(int w, int h, const char* title);
    virtual ~Window();
    gpu::RenderingContext* get_rendering_context();

private:
    gpu::RenderingContext* _gl;
};
#endif // VODK_WIDGET_SFML




#ifdef VODK_WIDGET_SDL

class Window {
public:
    struct Event {
        enum Type {
            Closed,
            KeyDown,
            Resized,
        };
        Type type;
    };

    Window(int w, int h, const char* title);
    virtual ~Window();
    gpu::RenderingContext* get_rendering_context();

    bool poll_events(Event& e);
    void display();
private:
    gpu::RenderingContext* _gl;
};

#endif // VODK_WIDGET_SDL




#ifdef VODK_WIDGET_MOCK

class Window {
public:
    typedef int Event;

    Window(int w, int h, const char* title) {}
    virtual ~Window() {}
    gpu::RenderingContext* get_rendering_context() { return nullptr; }
};

#endif // VODK_WIDGET_MOCK



} // io
} // vodk

#endif
