
#include "vodk/gfx/GfxSubSystem.hpp"
#include "vodk/data/GfxAssets.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gpu/quad.hpp"

namespace vodk {
namespace gfx {

void
BasicGfxSubSystem::render(float dt) {
    glm::mat4 mv(1.0);
    auto objects = range();
    printf("BasicGfxSubSystem::render %i\n", (int)objects.size());
    while (!objects.empty()) {
        printf("render basic object\n");
        BasicGfxComponent& c = objects.pop_front();
        glm::mat4 transform;

        gpu::ShaderProgram p = _shader->get_shader_program();
        _ctx->bind(p);
        _ctx->send_unirform(_ctx->get_uniform_location(p, "in_Texture"), 0, _tex->get_texture());
        _ctx->send_unirform(_ctx->get_uniform_location(p, "in_ModelView"), mv);
        _ctx->send_unirform(_ctx->get_uniform_location(p, "in_Transform"), c.transform);

        gpu::draw_unit_quad(_ctx);
    }
}

} // gfx
} // vodk
