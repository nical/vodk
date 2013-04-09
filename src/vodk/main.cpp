
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
    io::Window window(480, 320, "Vodk");
    gpu::RenderingContext* ctx = window.getRenderingContext();
    gpu::initQuad(ctx);
    data::InitImageAssetManager();

    assert(ctx->isSupported(gpu::FRAGMENT_SHADING));

    ctx->setClearColor(1.0, 0.5, 0.0, 1.0);

    data::ShaderAsset* vs_asset = new ShaderAsset(ctx, gpu::VERTEX_SHADER,
                                                  new data::StringAsset("assets/shaders/basic.vert"));
    data::ShaderAsset* fs_asset = new ShaderAsset(ctx, gpu::FRAGMENT_SHADER,
                                                  new data::StringAsset("assets/shaders/textured.frag"));
    data:;ShaderProgramAsset* program_asset = new data::ShaderProgramAsset(ctx, vs_asset, fs_asset);

    ImageAsset img_asset("assets/img/test.png");

    TextureAsset texture_asset(ctx, &img_asset);

    if (vs_asset->load() && fs_asset->load()) {
        if (!program_asset->load()) {
            printf("failed to build the shader program");
        }
    } else {
        printf("failed to compile the shaders");
    }

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

        gpu::ShaderProgram p = program_asset->getShaderProgram();
        ctx->bind(p);
        ctx->sendUniform(ctx->getUniformLocation(p, "in_Texture"), 0, texture_asset.getTexture());
        ctx->sendUniform(ctx->getUniformLocation(p, "in_ModelView"), model_view);
        ctx->sendUniform(ctx->getUniformLocation(p, "in_Transform"), transform);
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