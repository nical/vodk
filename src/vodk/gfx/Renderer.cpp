
#include "vodk/gfx/Renderer.hpp"
#include "vodk/gpu/RenderingContext.hpp"

namespace vodk {
namespace gfx {

void RenderList::push_front(RenderItem* item) {
    item->_next_item = _first_item;
    _first_item = item;
}

void RenderList::push_back(RenderItem* item) {
    if (!_first_item) {
        _first_item = item;
        return;
    }
    RenderItem* it = _first_item;
    while (it) {
        if (!it->_next_item) {
            it->_next_item = item;
            return;
        }
    }
}

bool ClearRenderItem::render(gpu::RenderingContext& ctx) {
    ctx.clear(gpu::ALL_BUFFERS);
}

} // gfx
} // vodk
