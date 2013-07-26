
#include "vodk/platform/Window.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gfx/ImageSurface.hpp"
#include "vodk/gpu/quad.hpp"
#include "vodk/data/GfxAssets.hpp"
#include "vodk/logic/AST.hpp"
#include "vodk/core/IDLookupVector.hpp"
#include "vodk/core/Timeline.hpp"
#include "vodk/gfx/GfxSubSystem.hpp"
#include "vodk/core/Scope.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <assert.h>

unsigned int loadFile(const char* path, char *& buffer);

using namespace vodk;
using namespace vodk::data;


gpu::RenderingContext* ctx;
io::Window* window;
data::ShaderProgramAsset* program_asset;
data::TextureAsset* texture_asset;

gfx::BasicGfxSubSystem* gfxComponents;
Scope* scope;

bool mainLoop() {
    printf("mainLoop\n");
    io::Window::Event event;
    while (window->poll_events(event)) {
        if (event.type == io::Window::Event::Closed) {
            return false;
        }
    }

    ctx->clear(gpu::targetBuffer(gpu::COLOR_BUFFER|gpu::DEPTH_BUFFER));

    glm::mat4 model_view(1.0);
    glm::mat4 transform(1.0);
/*
    gpu::ShaderProgram p = program_asset->get_shader_program();
    ctx->bind(p);
    ctx->send_unirform(ctx->get_uniform_location(p, "in_Texture"), 0, texture_asset->get_texture());
    ctx->send_unirform(ctx->get_uniform_location(p, "in_ModelView"), model_view);
    ctx->send_unirform(ctx->get_uniform_location(p, "in_Transform"), transform);
    gpu::draw_unit_quad(ctx);
*/
    gfxComponents->render(16.0);

    printf("foo\n");
    window->display();
    printf("bar\n");
    return true;
}


int main()
{
    vodk::unittest::IDLookupVector();
    vodk::unittest::Timeline();
    vodk::unittest::ast();


    // create the window
    window = new io::Window(480, 320, "Vodk");
    ctx = window->get_rendering_context();
    gpu::init_quad(ctx);
    data::InitImageAssetManager();
    ctx->set_viewport(0,0,480,320);

    scope = new Scope;

    assert(ctx->is_supported(gpu::FRAGMENT_SHADING));

    ctx->set_clear_color(1.0, 0.5, 0.0, 1.0);

    data::AssetManager mgr;

    data::ShaderAsset* vs_asset = new ShaderAsset(&mgr, ctx, gpu::VERTEX_SHADER,
                                                  new data::StringAsset(&mgr, "assets/shaders/basic.vert"));
    data::ShaderAsset* fs_asset = new ShaderAsset(&mgr, ctx, gpu::FRAGMENT_SHADER,
                                                  new data::StringAsset(&mgr, "assets/shaders/textured.frag"));
    program_asset = new data::ShaderProgramAsset(&mgr, ctx, vs_asset, fs_asset);

    ImageAsset img_asset(&mgr, "assets/img/test.png");

    texture_asset = new TextureAsset(&mgr, ctx, &img_asset);

    if (vs_asset->load() && fs_asset->load()) {
        if (!program_asset->load()) {
            printf("failed to build the shader program");
        }
    } else {
        printf("failed to compile the shaders");
    }

    if (img_asset.load()) {
        texture_asset->load();
    } else {
        printf("failed to load img/test.png\n");
    }

    auto e1 = scope->add_entity(EntityDescriptor(0.0, 0.0, 0.0, SYSTEM_GFX_BASIC));
    auto e2 = scope->add_entity(EntityDescriptor(-1.0, 0.0, 0.0, SYSTEM_GFX_BASIC));

    gfxComponents = new vodk::gfx::BasicGfxSubSystem(nullptr, 1, ctx, program_asset, texture_asset);
    gfx::BasicGfxComponent gfxcomp1(0, glm::mat4(1.0));
    glm::mat4 m1(1.0);
    gfx::BasicGfxComponent gfxcomp2(1, glm::translate(m1, glm::vec3(-1.0, 0.0, 0.0)));
    //gfxComponents->add(gfxcomp1);
    //gfxComponents->add(gfxcomp2);

#ifndef EMSCRITEN
    // main loop
    while (mainLoop()) {}
#else
    mainLoop();
#endif

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