// Copyright (c) 2013 Nicolas Silva
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
    StringAsset(AssetManager* mgr, const char* url)
    : Asset(mgr)
    {
        _path = url;
        set_state(Asset::State::LOADED);
    }

    virtual AssetType get_type() override {
        return STRING_ASSET;
    }

    virtual bool load() override {}
    virtual void unload() override {}

    std::string& get_string() {
        return _path;
    }
protected:
    std::string _path;
};

class ImageAsset : public data::Asset
{
public:
    static const AssetType Type = IMAGE_ASSET;
    ImageAsset(AssetManager* mgr, const char* path);

    virtual AssetType get_type() override {
        return IMAGE_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gfx::ImageSurface* get_image() {
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
    TextureAsset(AssetManager* mgr, gpu::RenderingContext* rc, ImageAsset* dep);

    virtual AssetType get_type() override {
        return TEXTURE_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gpu::Texture& get_texture() {
        return _tex;
    }
    gpu::RenderingContext* get_rendering_context() {
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
    ShaderAsset(AssetManager* mgr, gpu::RenderingContext* rc, gpu::ShaderType type, StringAsset* dep)
    : Asset(mgr), _ctx(rc), _type(type)
    {
        if (!add_dependency(dep)) { assert(false); }
        _shader = _ctx->create_shader(type);
    }

    virtual AssetType get_type() override {
        return SHADER_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;


    gpu::Shader& get_shader() {
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
    ShaderProgramAsset(AssetManager* mgr, gpu::RenderingContext* rc, ShaderAsset* vs, ShaderAsset* fs)
    : Asset(mgr), _ctx(rc)
    {
        if (!add_dependency(vs, 0)) { assert(false); }
        if (!add_dependency(fs, 1)) { assert(false); }
        _program = _ctx->create_shaderProgram();
    }

    virtual AssetType get_type() override {
        return SHADER_PROGRAM_ASSET;
    }

    virtual bool load() override;
    virtual void unload() override;

    gpu::ShaderProgram& get_shader_program() {
        return _program;
    }

protected:
    gpu::RenderingContext* _ctx;
    gpu::ShaderProgram _program;
};

} // data
} // vodk

#endif
