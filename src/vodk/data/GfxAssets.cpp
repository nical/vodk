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

#include "vodk/core/config.hpp"
#include "GfxAssets.hpp"
#include "vodk/gfx/ImageSurface.hpp"
#include "vodk/parser/png.hpp"
#include <memory.h>
#include <assert.h>

unsigned int loadFile(const char* path, char *& buffer);

namespace vodk {
namespace data {


static gfx::ImageSurface* sDefaultImage = nullptr;

void InitImageAssetManager() {
    assert(sDefaultImage == nullptr);
    
    sDefaultImage = new gfx::ImageSurface();

    sDefaultImage->allocate(gfx::IntSize(32,32), gfx::SURFACE_R8G8B8A8);
    assert(sDefaultImage->getSize() == gfx::IntSize(32,32));
    assert(sDefaultImage->getData() != nullptr);

    uint8_t* pixels = sDefaultImage->getData();

    for(int j = 0; j<32; ++j) {
        for(int i = 0; i<32; ++i) {
            if ( (i/2 % 2 == 0)^(j/2 % 2 == 0) ) {
                pixels[j * 32 * 4 + i * 4 + 0] = 255;
                pixels[j * 32 * 4 + i * 4 + 1] = 255;
                pixels[j * 32 * 4 + i * 4 + 2] = 255;
            } else {
                pixels[j * 32 * 4 + i * 4 + 0] = 100;
                pixels[j * 32 * 4 + i * 4 + 1] = 100;
                pixels[j * 32 * 4 + i * 4 + 2] = 100;
            }
            pixels[j * 32 * 4 + i * 4 + 3] = 255;
        }
    }
}

bool ImageAsset::load()
{
    printf("ImageAsset::load\n");

    parser::PNGImage png;
    if (png.loadFromFile(_path.c_str())) {
        if (!_img || _img == sDefaultImage) {
            _img = new gfx::ImageSurface;
        }
        png.copyInto(*_img);
    }

    notifyLoaded();
    return true;
}

void ImageAsset::unload()
{
    printf("ImageAsset::unload\n");
    if (_img && _img != sDefaultImage) {
        delete _img;
    }
    _img = sDefaultImage;
    invalidate();
}

ImageAsset::ImageAsset(AssetManager* mgr, const char* path)
: Asset(mgr), _path(path), _img(sDefaultImage)
{
    assert(sDefaultImage);
    setState(Asset::State::DEFAULT);
}

TextureAsset::TextureAsset(AssetManager* mgr, gpu::RenderingContext* rc, ImageAsset* dep)
: Asset(mgr), _ctx(rc)
{
    if (!addDependency(dep)) { assert(false); }
    _tex = _ctx->createTexture();
}

bool TextureAsset::load()
{
    printf("TextureAsset::load\n");

    Asset* asset = &*dependencies();
    assert(asset->getType() == IMAGE_ASSET);
    ImageAsset* img_asset = static_cast<ImageAsset*>(asset);

    _ctx->upload(_tex, img_asset->getImage());
    notifyLoaded();
}

void TextureAsset::unload()
{
    invalidate();
}

bool ShaderAsset::load()
{
    StringAsset* str_asset = castAsset<StringAsset>(&*dependencies());
    assert(str_asset);

    char* src;
    int len = loadFile(str_asset->getString().c_str(), src);

    if (len > 0) {
        gpu::ShaderSource vs(1, &src, &len);
        bool result = _ctx->compileShader(_shader, vs);

        delete[] src;
        return result;
    } else printf("failed to find file %s\n", str_asset->getString().c_str());

    delete[] src;
    return false;
}

void ShaderAsset::unload()
{

}

bool ShaderProgramAsset::load()
{
    AssetConnectionIterator it = dependencies();
    ShaderAsset* vs_asset = castAsset<ShaderAsset>(&*it);
    it = it.next();
    ShaderAsset* fs_asset = castAsset<ShaderAsset>(&*it);

    if (!vs_asset || !fs_asset) return false;
    if (vs_asset == fs_asset) return false;

    _ctx->attachShader(_program, vs_asset->getShader());
    _ctx->attachShader(_program, fs_asset->getShader());
    _ctx->bindAttributeLocation(_program, 0, "in_Position");
    _ctx->bindAttributeLocation(_program, 1, "in_TexCoords");
    _ctx->linkShaderProgram(_program);

    return true;
}

void ShaderProgramAsset::unload()
{

}


} // data
} // vodk
