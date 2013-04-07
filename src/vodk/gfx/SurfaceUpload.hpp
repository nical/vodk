
#ifndef VODK_GFX_SURFACEUPLOAD_HPP
#define VODK_GFX_SURFACEUPLOAD_HPP

#include "vodk/gpu/RenderingContext.hpp"

namespace gfx {

class Surface;

bool uploadSurface(gpu::RenderingContext* ctx,
                   gfx::Surface* src,
                   const gpu::Texture& dest,
                   gpu::UploadFlags flags = UPLOAD_DEFAULT);


} // namespace

#endif
