
#ifndef VODK_GPU_QUAD_HPP
#define VODK_GPU_QUAD_HPP

namespace vodk {
namespace gpu {

class RenderingContext;

void init_quad(gpu::RenderingContext* ctx);
void draw_unit_quad(gpu::RenderingContext* ctx);

} // namespace gpu
} // namespace vodk

#endif
