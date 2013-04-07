
#ifndef VODK_DATA_GFXASSETS_HPP
#define VODK_DATA_GFXASSETS_HPP

#include "vodk/data/Asset.hpp"
#include "vodk/gpu/RenderingContext.hpp"

#include <string>

namespace vodk {

namespace gfx {
class ImageSurface;
}

namespace data {

void InitImageAssetManager();

class ImageAsset : public data::Asset
{
public:
    ImageAsset(const char* path);

    virtual AssetType getType() override {
        return IMAGE_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gfx::ImageSurface* getImage() {
        return _img;
    }
protected:
    std::string _path;
    gfx::ImageSurface* _img;
};

class TextureAsset : public data::Asset
{
public:
    TextureAsset(gpu::RenderingContext* rc, ImageAsset* dep);

    virtual AssetType getType() override {
        return TEXTURE_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gpu::Texture& getTexture() {
        return _tex;
    }
    gpu::RenderingContext* getRenderingContext() {
        return _ctx;
    }
protected:
    gpu::RenderingContext* _ctx;
    gpu::Texture _tex;
};

} // data
} // vodk

#endif
