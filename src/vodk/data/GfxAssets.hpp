
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

class StringAsset : public data::Asset
{
public:
    static const AssetType Type = STRING_ASSET;
    StringAsset(const char* url) {
        _path = url;
        setState(Asset::State::LOADED);
    }

    virtual AssetType getType() override {
        return STRING_ASSET;
    }

    virtual bool load() override {}
    virtual void unload() override {}

    std::string& getString() {
        return _path;
    }
protected:
    std::string _path;
};

class ImageAsset : public data::Asset
{
public:
    static const AssetType Type = IMAGE_ASSET;
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
    static const AssetType Type = TEXTURE_ASSET;
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

class ShaderAsset : public data::Asset
{
public:
    static const AssetType Type = SHADER_ASSET;
    ShaderAsset(gpu::RenderingContext* rc, gpu::ShaderType type, StringAsset* dep)
    : _ctx(rc), _type(type)
    {
        if (!addDependency(dep)) { assert(false); }
        _shader = _ctx->createShader(type);
    }

    virtual AssetType getType() override {
        return SHADER_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;


    gpu::Shader& getShader() {
        return _shader;
    }

protected:
    gpu::RenderingContext* _ctx;
    gpu::Shader _shader;
    gpu::ShaderType _type;
};

class ShaderProgramAsset : public data::Asset
{
public:
    static const AssetType Type = SHADER_PROGRAM_ASSET;
    ShaderProgramAsset(gpu::RenderingContext* rc, ShaderAsset* vs, ShaderAsset* fs)
    : _ctx(rc)
    {
        if (!addDependency(vs, 0)) { assert(false); }
        if (!addDependency(fs, 1)) { assert(false); }
        _program = _ctx->createShaderProgram();
    }

    virtual AssetType getType() override {
        return SHADER_PROGRAM_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gpu::ShaderProgram& getShaderProgram() {
        return _program;
    }

protected:
    gpu::RenderingContext* _ctx;
    gpu::ShaderProgram _program;
};

} // data
} // vodk

#endif
