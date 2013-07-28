
#ifndef VODK_GFX_GFXSUBSYSTEM_HPP
#define VODK_GFX_GFXSUBSYSTEM_HPP

#include "vodk/core/SubSystem.hpp"
#include "vodk/core/Entity.hpp"

#include <glm/glm.hpp>

namespace vodk {
class Scope;

namespace data {
class ShaderProgramAsset;
class TextureAsset;
} // data

namespace gpu {
class RenderingContext;
}

namespace gfx {

/**
 * Common interface for gfx SubSystems.
 */
class GfxSubSystem {
public:
    virtual void render(float dt) = 0;
};

struct BasicGfxComponent {
    BasicGfxComponent() {}
    BasicGfxComponent(EntityOffset aEntity, glm::mat4 aTransform)
    : entity(aEntity), transform(aTransform) {}

    glm::mat4 transform;
    ObjectID::Index index;
    EntityOffset entity;
    ObjectID::GenHash genHash;
};

/**
 * A very simple gfx system for testing purposes.
 */
class BasicGfxSubSystem : public TSubSystem<BasicGfxComponent>
                        , public GfxSubSystem
{
public:
    typedef TSubSystem<BasicGfxComponent> Parent;

    BasicGfxSubSystem(Scope* s,
                      gpu::RenderingContext* context,
                      data::ShaderProgramAsset* shader,
                      data::TextureAsset* texture)
    : Parent()
    , _scope(s)
    , _ctx(context)
    , _shader(shader)
    , _tex(texture)
    {}

    virtual SubSystemID id() const { return SYSTEM_GFX_BASIC; }

    virtual void render(float dt) override;

    virtual void update(float dt) override {}

protected:
    Scope* _scope;
    gpu::RenderingContext* _ctx;
    data::ShaderProgramAsset* _shader;
    data::TextureAsset* _tex;
};

} // namespace gfx
} // namespace vodk

#endif
