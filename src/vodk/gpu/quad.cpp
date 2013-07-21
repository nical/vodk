
#include "vodk/gpu/quad.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gpu/ScopedBindObject.hpp"

namespace vodk {
namespace gpu {

static gpu::VertexArray _vao(gpu::STATIC_UPDATE);
static gpu::VertexBuffer _vertex_vbo(gpu::STATIC_UPDATE);
static gpu::VertexBuffer _texcoord_vbo(gpu::STATIC_UPDATE);

void initQuad(RenderingContext* ctx) {
    float vertices[12] = {
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0
    };
    float tex_coords[8] = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 1.0,
        1.0, 0.0
    };

    _vao = ctx->createVertexArray();
    ScopedBind<VertexArray> bind_vao(ctx, _vao);

    // vertices
    {
        _vertex_vbo = ctx->createVertexBuffer();
        ScopedBind<VertexBuffer> bind_vbo(ctx, _vertex_vbo);
        ctx->upload(_vertex_vbo, range(vertices, 12).bytes());
        ctx->defineVertexAttribute(0, gpu::FLOAT, 3, 0, 0);
        ctx->enableVertexAttribute(0);
    }

    // texture coordinates
    {    
        _texcoord_vbo = ctx->createVertexBuffer();
        ScopedBind<VertexBuffer> bind_vbo(ctx, _texcoord_vbo);
        ctx->upload(_texcoord_vbo, range(tex_coords, 8).bytes());
        ctx->defineVertexAttribute(1, gpu::FLOAT, 2, 0, 0);
        ctx->enableVertexAttribute(1);
    }
}

void drawUnitQuad(RenderingContext* ctx) {
    ctx->enableVertexAttribute(0);
    ctx->enableVertexAttribute(1);
    gpu::ScopedBind<VertexArray> bind_vao(ctx, _vao);
    ctx->drawArrays(gpu::TRIANGLE_STRIP, 0, 4);
}


} // namespace gpu
} // namespace vodk
