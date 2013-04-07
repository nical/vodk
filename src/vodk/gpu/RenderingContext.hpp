
#ifndef VODK_GPU_RENDERINGCONTEXT_HPP
#define VODK_GPU_RENDERINGCONTEXT_HPP

#include "vodk/core/config.hpp"
#include "vodk/core/Slice.hpp"
#include <stdint.h>
#include <glm/glm.hpp>

namespace vodk {

namespace gfx {
class Surface;
} // namespace

namespace gpu {

enum BackendType {
    OPENGL_BACKEND,
    INVALID_BACKEND
};

enum UpdateHint {
    STATIC_UPDATE,
    STREAM_UPDATE,
    DYNAMIC_UPDATE
};

enum BufferType {
    ARRAY_BUFFER,
    PIXEL_BUFFER,
    INDEX_BUFFER
};

enum ShaderType {
    FRAGMENT_SHADER,
    VERTEX_SHADER,
    GEOMETRY_SHADER,
};

enum UploadFlags {
    UPLOAD_DEFAULT   = 0,
    UPLOAD_NORMALIZE = 1,
    UPLOAD_ASYNC     = 2
};

enum TextureFlags {
    TEXTURE_REPEAT,
    TEXTURE_CLAMP,
    TEXTURE_SAMPLE_LINEAR,
    TEXTURE_SAMPLE_NEAREST,
};

enum TargetBuffer {
    COLOR_BUFFER    = 1,
    DEPTH_BUFFER    = 2,
    STENCIL_BUFFER  = 4
};

enum AttributeType {
    FLOAT,
    DOUBLE,
    INT,
    UNSIGNED_INT
};

enum DrawMode {
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP
};

enum Feature {
    FRAGMENT_SHADING,
    VERTEX_SHADING,
    GEOMETRY_SHADING,
    RENDER_TO_TEXTURE,
    MULTIPLE_RENDER_TARGETS,
};

TargetBuffer targetBuffer(int v);

typedef uint64_t Handle;

struct GPUObject {
  Handle handle;
  GPUObject(Handle h) : handle(h) {}
};

struct VertexBuffer  : public GPUObject {
    VertexBuffer(UpdateHint h, Handle id = 0)
    : GPUObject(id), update(h) {}
    gpu::UpdateHint update;
};

struct ElementBuffer  : public GPUObject {
    ElementBuffer(UpdateHint h, Handle id = 0)
    : GPUObject(id), update(h) {}
    gpu::UpdateHint update;
};

struct RenderTarget : public GPUObject { RenderTarget(Handle id = 0) : GPUObject(id) {}; };
struct GeometryBuffer : public GPUObject { GeometryBuffer(Handle id = 0) : GPUObject(id) {}; };
struct VertexArray : public GPUObject { VertexArray(Handle id = 0) : GPUObject(id) {} };
struct Shader : public GPUObject {  Shader(Handle id = 0) : GPUObject(id) {} };
struct ShaderProgram: public GPUObject { ShaderProgram(Handle id = 0) : GPUObject(id) {} };
struct Texture : public GPUObject { Texture(Handle id = 0) : GPUObject(id) {} };

struct ShaderSource
{
    ShaderSource(int c, const char** s, const int* l)
    : count(c), str(s), len(l) {}
    ShaderSource(int c, char** s, int* l)
    : count(c), str((const char**)s), len((const int*)l) {}
    int count;
    const char** str;
    const int* len;
};

struct GeometryRange {
    GeometryRange(const GeometryBuffer& buf, int32_t s, int32_t e)
    : buffer(buf), start(s), end(e) {}
    GeometryBuffer buffer;
    int32_t start;
    int32_t end;
};

struct TextureDescriptor {

};

class ShaderInputIterator {
    
};

class RenderingContext {
public:
    typedef uint8_t byte;

    virtual ~RenderingContext() {}

    virtual void setViewport(int x, int y, int w, int h) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear(TargetBuffer target) = 0;

    virtual VertexArray createVertexArray() = 0;
    virtual void deleteVertexArray(VertexArray& vao) = 0;
    virtual void bind(const VertexArray& vao) = 0;
    virtual void unbind(const VertexArray& vao) = 0;
    /**
     * Enable a given vertex attribute location to be used with a VertexBuffer
     * as input.
     * the values in the generic vertex attribute array will be accessed and 
     * used for rendering when calls are made to drawArrays.
     */
    virtual void enableVertexAttribute(int index) = 0;
    virtual void disableVertexAttribute(int index) = 0;
    virtual VertexBuffer createVertexBuffer() = 0;
    virtual void deleteVertexBuffer(VertexBuffer& vbo) = 0;
    virtual void bind(const VertexBuffer& vbo) = 0;
    virtual void unbind(const VertexBuffer& vbo) = 0;
    virtual void upload(const VertexBuffer& buf,
                        Slice<byte> data,
                        UploadFlags flags = UPLOAD_DEFAULT) = 0;

    virtual gpu::Texture createTexture(gpu::UpdateHint hint = gpu::STATIC_UPDATE) = 0;
    virtual void deleteTexture(gpu::Texture tex) = 0;
    virtual void bind(gpu::Texture tex) = 0;
    virtual void unbind(gpu::Texture tex) = 0;
    virtual void setTextureFlags(gpu::Texture tex, TextureFlags flags) = 0;
    virtual void upload(gpu::Texture dest,
                        gfx::Surface* src,
                        UploadFlags flags = UPLOAD_DEFAULT) = 0;
    virtual void defineVertexAttribute(int attribIndex,
                                       gpu::AttributeType type,
                                       int compPerVertex,
                                       int stride = 0, // zero means tightly packed attributes (=sizeof(type))
                                       int offset = 0) = 0;

    virtual gpu::Shader createShader(gpu::ShaderType type) = 0;
    virtual gpu::ShaderProgram createShaderProgram() = 0;
    virtual void deleteShader(gpu::Shader shader) = 0;
    virtual bool compileShader(gpu::Shader, const ShaderSource& src) = 0;
    virtual void bind(gpu::ShaderProgram s) = 0;
    virtual void unbind(gpu::ShaderProgram s) = 0;
    virtual void attachShader(gpu::ShaderProgram p, gpu::Shader s) = 0;
    virtual bool linkShaderProgram(gpu::ShaderProgram p) = 0;
    virtual bool bindAttributeLocation(gpu::ShaderProgram p,
                                       int location,
                                       const char* name) = 0;
    virtual void sendUniform(int location, float val) = 0;
    virtual void sendUniform(int location, const glm::vec2& val) = 0;
    virtual void sendUniform(int location, const glm::vec3& val) = 0;
    virtual void sendUniform(int location, const glm::vec4& val) = 0;
    virtual void sendUniform(int location, const glm::mat4& val) = 0;
    virtual void sendUniform(int uniformLocation,
                             int textureUnit,
                             const Texture& tex) = 0;

    virtual int getUniformLocation(const ShaderProgram& p, const char* name) = 0;

    virtual void drawArrays(gpu::DrawMode mode, int first, int count) = 0;

    virtual bool isSupported(gpu::Feature feature) = 0;
    virtual bool isSupported(const char* str) = 0; // platform specific string
    virtual void makeCurrent() = 0;

};

class RenderingContext2 {
public:
    virtual void setViewport(int x, int y, int w, int h) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear(TargetBuffer target) = 0;

    virtual void draw(const RenderTarget& target,
                      const GeometryRange& vertices,
                      ShaderInputIterator* inputs,
                      const Shader& shader,
                      DrawMode mode = gpu::TRIANGLES) = 0;
    virtual Texture createTexture(const TextureDescriptor& desc);
};

} // namespace
} // namespace

#endif


/*

ShaderProgramDescriptor desc{
    vertexShader: &vs_src,
    fragmentShader: &fs_src,
}
ShaderProgramResult res;
ctx->buildShaderProgram(program, desc, res);

RenderTargetDescriptor rtDesc{
    color: Texture
    stencil: Texture
    depth: Texture
}

rt = ctx->CreateRenderTarget(rtDesc)

GeometryDescriptor

ctx->draw(renderTarget, shader, geometry, start, end)



*/