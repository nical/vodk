
#ifndef VODK_GFX_RENDERER_HPP
#define VODK_GFX_RENDERER_HPP

namespace vodk {
namespace gpu {
class RenderingContext;
} // gpu
} // vodk

namespace vodk {
namespace gfx {

class RenderList;

class RenderItem {
public:
	RenderItem() : _next_item(nullptr) {}

	virtual ~RenderItem() { delete _next_item; }

	virtual RenderList* as_render_list() { return nullptr; }

	virtual bool render(gpu::RenderingContext& ctx) = 0;

	RenderItem* _next_item;
};

class RenderList : public RenderItem {
public:
	RenderList() : _first_item(nullptr) {}

	~RenderList() { delete _first_item; }

	virtual RenderList* as_render_list() { return this; }

	virtual bool render(gpu::RenderingContext& ctx) override {
		RenderItem* it = _first_item;
		while (it) {
			it->render(ctx);
			it = it->_next_item;
		}
	}

	void push_front(RenderItem* item);

	void push_back(RenderItem* item);

	RenderItem* _first_item;
};

class ClearRenderItem : public RenderItem {
public:
	virtual bool render(gpu::RenderingContext& ctx) override;
};

class Renderer {
public:
	virtual void add_render_item(RenderItem* item) = 0;
};

class RemoteRenderer : public Renderer {
public:
	virtual void add_render_item(RenderItem* item) override;
};

class HostRenderer : public Renderer {
public:
	HostRenderer()
	: _render_list(new RenderList())
	{}

	virtual void add_render_item(RenderItem* item) override {
		_render_list->push_back(item);
	}

	void clear_render_items() {
		delete _render_list;
	}

	RenderList* _render_list;
	gpu::RenderingContext* _ctx;
};

} // gfx
} // vodk

#endif
