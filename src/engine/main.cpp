
#include "vodk/platform/Window.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gfx/ImageSurface.hpp"
#include "vodk/gpu/quad.hpp"
#include "vodk/data/GfxAssets.hpp"
#include "vodk/logic/AST.hpp"
#include "vodk/core/IDLookupVector.hpp"
#include "vodk/core/Timeline.hpp"
#include "vodk/gfx/GfxSubSystem.hpp"
#include "vodk/gfx/ImageSurface.hpp"
#include "vodk/core/Scope.hpp"
#include "vodk/io/file.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <assert.h>
#include <vector>


namespace vodk {
namespace gfx {
bool vectorize(vodk::gfx::ImageSurface* in, std::vector<glm::vec2>& out);
}
}

using namespace vodk;
using namespace vodk::data;


gpu::RenderingContext* ctx;
io::Window* window;
data::ShaderProgramAsset* program_asset;
data::TextureAsset* texture_asset;

gfx::BasicGfxSubSystem* basic_gfx;
Scope* scope;

bool main_loop() {
    printf("main_loop\n");
    io::Window::Event event;
    while (window->poll_events(event)) {
        if (event.type == io::Window::Event::Closed) {
            return false;
        }
    }

    ctx->clear(gpu::targetBuffer(gpu::COLOR_BUFFER|gpu::DEPTH_BUFFER));

    basic_gfx->render(16.0);

    window->display();
    scope->flush();

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

    ImageAsset img_asset(&mgr, "assets/img/test2.png");

    texture_asset = new TextureAsset(&mgr, ctx, &img_asset);

    if (vs_asset->load() && fs_asset->load()) {
        if (!program_asset->load()) {
            printf("failed to build the shader program");
        }
    } else {
        printf("failed to compile the shaders");
    }

    if (img_asset.load()) {
        std::vector<glm::vec2> points;
        vodk::gfx::vectorize(img_asset.get_image(), points);
        texture_asset->load();
    } else {
        printf("failed to load img/test.png\n");
    }

    basic_gfx = new vodk::gfx::BasicGfxSubSystem(
        scope, ctx, program_asset, texture_asset
    );
    scope->add_subsystem(basic_gfx);

    scope->add_entity_async(EntityDescriptor(
        0.0, 0.0, 0.0,
        SYSTEM_GFX_BASIC,
        SYSTEM_NONE,
        SYSTEM_NONE,
        SYSTEM_NONE
    ));
    scope->add_entity_async(EntityDescriptor(
        -1.0, 0.0, 0.0,
        SYSTEM_GFX_BASIC,
        SYSTEM_NONE,
        SYSTEM_NONE,
        SYSTEM_NONE
    ));

    scope->flush();

#ifndef EMSCRITEN
    // main loop
    while (main_loop()) {}
#else
    main_loop();
#endif

    return 0;
}

