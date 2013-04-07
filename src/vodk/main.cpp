
#include "vodk/platform/Window.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gfx/ImageSurface.hpp"
#include "vodk/gpu/quad.hpp"
#include "vodk/data/GfxAssets.hpp"

#include <stdio.h>
#include <assert.h>

unsigned int loadFile(const char* path, char *& buffer);

using namespace vodk;
using namespace vodk::data;

int main()
{
    // create the window
    io::Window window(320, 480, "OpenGL");
    gpu::RenderingContext* ctx = window.getRenderingContext();
    gpu::initQuad(ctx);
    data::InitImageAssetManager();

    assert(ctx->isSupported(gpu::FRAGMENT_SHADING));

    ctx->setClearColor(1.0, 0.5, 0.0, 1.0);

    char* src;
    gpu::Shader fragment_shader;
    gpu::Shader vertex_shader;
    gpu::ShaderProgram program;
    int len1 = loadFile("shaders/basic.vert", src);
    if (len1 > 0) {
        gpu::ShaderSource vs(1, &src, &len1);
        vertex_shader = ctx->createShader(gpu::VERTEX_SHADER);
        assert(ctx->compileShader(vertex_shader, vs));
    } else printf("failed to load basic.vert\n");
    int len2 = loadFile("shaders/textured.frag", src);
    if (len2 > 0) {
        gpu::ShaderSource fs(1, &src, &len2);
        fragment_shader = ctx->createShader(gpu::FRAGMENT_SHADER);
        assert(ctx->compileShader(fragment_shader, fs));
    } else printf("failed to load basic.frag\n");
    if (len1 > 0 && len2 > 0) {
        program = ctx->createShaderProgram();
        ctx->attachShader(program, vertex_shader);
        ctx->attachShader(program, fragment_shader);
        ctx->bindAttributeLocation(program, 0, "in_Position");
        ctx->bindAttributeLocation(program, 1, "in_TexCoords");
        ctx->linkShaderProgram(program);
    }

    ImageAsset img_asset("img/test.png");
    TextureAsset texture_asset(ctx, &img_asset);

    if (img_asset.load()) {
        texture_asset.load();
    } else {
        printf("failed to load img/test.png\n");
    }

    glm::mat4 model_view(1.0);
    glm::mat4 transform(1.0);

    // main loop
    bool running = true;
    while (running) {
        io::Window::Event event;
        while (window.pollEvent(event)) {
            if (event.type == io::Window::Event::Closed) {
                running = false;
            }
#ifdef VODK_WIDGET_SFML
            else if (event.type == sf::Event::Resized) {
                ctx->setViewport(0, 0, event.size.width, event.size.height);
            }
#endif // SFML
        }

        ctx->clear(gpu::targetBuffer(gpu::COLOR_BUFFER|gpu::DEPTH_BUFFER));

        ctx->bind(program);
        ctx->sendUniform(ctx->getUniformLocation(program, "in_Texture"), 0, texture_asset.getTexture());
        ctx->sendUniform(ctx->getUniformLocation(program, "in_ModelView"), model_view);
        ctx->sendUniform(ctx->getUniformLocation(program, "in_Transform"), transform);
        gpu::drawUnitQuad(ctx);

        window.display();

#ifdef EMSCRIPTEN
        running = false;
#endif
    }

    return 0;
}

#include <iostream>
#include <fstream>
unsigned int loadFile(const char* path, char *& buffer)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        buffer = nullptr;
        return -1;
    }
    // get length of file:
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    // allocate memory:
    buffer = new char [length];

    // read data as a block:
    file.read(buffer,length);
    file.close();

    return length;
}