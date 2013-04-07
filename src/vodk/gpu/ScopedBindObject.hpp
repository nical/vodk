
#ifndef VODK_GPU_SCOPEDBINDOBJECT_HPP
#define VODK_GPU_SCOPEDBINDOBJECT_HPP

#include "vodk/gpu/RenderingContext.hpp"
#include <stdio.h>

namespace vodk {
namespace gpu {

template<typename Buffer>
struct ScopedBind {
    ScopedBind(RenderingContext* ctx, Buffer& buf)
    : _ctx(ctx), _buffer(&buf)
    {
        _ctx->bind(*_buffer);
    }

    ~ScopedBind() {
        _ctx->unbind(*_buffer);
    }

    RenderingContext* _ctx;
    Buffer* _buffer;
};

} // gpu
} // vodk

#endif
