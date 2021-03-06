
#include "vodk/gpu/quad.hpp"
#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gpu/ScopedBindObject.hpp"

namespace vodk {
namespace gpu {

static gpu::VertexArray _vao(gpu::STATIC_UPDATE);
static gpu::VertexBuffer _vertex_vbo(gpu::STATIC_UPDATE);
static gpu::VertexBuffer _texcoord_vbo(gpu::STATIC_UPDATE);

void init_quad(RenderingContext* ctx) {
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

    _vao = ctx->create_vertex_array();
    ScopedBind<VertexArray> bind_vao(ctx, _vao);

    // vertices
    {
        _vertex_vbo = ctx->create_vertex_buffer();
        ScopedBind<VertexBuffer> bind_vbo(ctx, _vertex_vbo);
        ctx->upload(_vertex_vbo, range(vertices, 12).bytes());
        ctx->define_vertex_attribute(0, gpu::FLOAT, 3, 0, 0);
        ctx->enable_vertex_attribute(0);
    }

    // texture coordinates
    {    
        _texcoord_vbo = ctx->create_vertex_buffer();
        ScopedBind<VertexBuffer> bind_vbo(ctx, _texcoord_vbo);
        ctx->upload(_texcoord_vbo, range(tex_coords, 8).bytes());
        ctx->define_vertex_attribute(1, gpu::FLOAT, 2, 0, 0);
        ctx->enable_vertex_attribute(1);
    }
}

void draw_unit_quad(RenderingContext* ctx, DrawMode mode) {
    ctx->enable_vertex_attribute(0);
    ctx->enable_vertex_attribute(1);
    gpu::ScopedBind<VertexArray> bind_vao(ctx, _vao);
    ctx->draw_arrays(mode, 0, 4);
}

} // namespace gpu
} // namespace vodk
