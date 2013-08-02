
#ifndef VODK_GPU_QUAD_HPP
#define VODK_GPU_QUAD_HPP

#include "vodk/gpu/RenderingContext.hpp"

namespace vodk {
namespace gpu {

class RenderingContext;

void init_quad(gpu::RenderingContext* ctx);
void draw_unit_quad(gpu::RenderingContext* ctx, DrawMode mode = gpu::TRIANGLE_STRIP);

} // namespace gpu
} // namespace vodk

#endif
