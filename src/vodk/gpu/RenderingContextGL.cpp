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

#ifdef VODK_GL_3

#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gpu/ScopedBindObject.hpp"
#include "vodk/gfx/Surface.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <memory.h>

#define CHECK_GL_ERROR check_gl_error(__LINE__,__PRETTY_FUNCTION__);

namespace vodk {

namespace io {
class Window;
}

namespace gpu {

class RenderingContextGL : public RenderingContext {
public:
    virtual void set_viewport(int x, int y, int w, int h) override;
    virtual void set_clear_color(float r, float g, float b, float a) override;
    virtual void clear(TargetBuffer target) override;

    virtual gpu::VertexArray create_vertex_array() override;
    virtual void delete_vertex_array(VertexArray& vao) override;
    virtual void bind(const VertexArray& vao) override;
    virtual void unbind(const VertexArray& vao) override;
    virtual gpu::VertexBuffer create_vertex_buffer() override;
    virtual void delete_vertex_buffer(VertexBuffer& vbo) override;
    virtual void bind(const VertexBuffer& vbo) override;
    virtual void unbind(const VertexBuffer& vbo) override;
    virtual void upload(const VertexBuffer& buf, Range<byte> data, UploadFlags flags = UPLOAD_DEFAULT);

    virtual gpu::Texture create_texture(gpu::UpdateHint hint) override;
    virtual void delete_texture(gpu::Texture tex) override;
    virtual void bind(gpu::Texture tex) override;
    virtual void unbind(gpu::Texture tex) override;
    virtual void set_texture_flags(gpu::Texture tex, TextureFlags flags) override;
    virtual void upload(gpu::Texture dest, gfx::Surface* src, UploadFlags flags) override;
    virtual void define_vertex_attribute(int attribIndex,
                                       gpu::AttributeType type,
                                       int compPerVertex,
                                       int stride,
                                       int offset) override;
    virtual void enable_vertex_attribute(int index) override;
    virtual void disable_vertex_attribute(int index) override;


    virtual gpu::Shader create_shader(gpu::ShaderType type) override;
    virtual gpu::ShaderProgram create_shaderProgram() override;
    virtual void delete_shader(gpu::Shader shader) override;
    virtual bool compile_shader(gpu::Shader, const ShaderSource& src) override;
    virtual void bind(gpu::ShaderProgram s) override;
    virtual void unbind(gpu::ShaderProgram s) override;
    virtual void attach_shader(gpu::ShaderProgram p, gpu::Shader s) override;
    virtual bool link_shader_program(gpu::ShaderProgram p) override;
    virtual bool bind_attribute_location(gpu::ShaderProgram p,
                                       int location,
                                       const char* name) override;
    virtual void send_unirform(int location, float val) override;
    virtual void send_unirform(int location, const glm::vec2& val) override;
    virtual void send_unirform(int location, const glm::vec3& val) override;
    virtual void send_unirform(int location, const glm::vec4& val) override;
    virtual void send_unirform(int location, const glm::mat4& val) override;
    virtual void send_unirform(int location, int texUnit,
                             const Texture& tex) override;

    virtual int get_uniform_location(const ShaderProgram& p, const char* name) override;

    virtual void draw_arrays(gpu::DrawMode mode, int first, int count) override;

    virtual bool is_supported(gpu::Feature feature) override;
    virtual bool is_supported(const char* str) override;
    virtual void make_current() override;
};

const char* glErrorToStr(GLenum err) {
    switch (err) {
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE";
        default: return "";
    }
}

static bool check_gl_error(int line, const char* comment)
{
    GLuint err = glGetError();
    if( err != GL_NO_ERROR )
    {
        printf("GL error %s at line %i of %s \n",
               glErrorToStr(err), line, comment);
        return false;
    }
    return true;
}


RenderingContextGL* createRenderingContext(io::Window* w) {
    return new RenderingContextGL();
}

void deleteRenderingContext(RenderingContext* ctx) {
    delete ctx;
}

static bool supportsOpenGL_2_0() {
    return GLEW_VERSION_2_0;
}
static bool supportsOpenGL_3_0() {
    return GLEW_VERSION_3_0;
}
static bool supportsOpenGL_3_3() {
    return GLEW_VERSION_3_3;
}

bool initBackend() {
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("Error when initializing OpenGL: %s\n", glewGetErrorString(err));
        return false;
    }
    if (!GLEW_ARB_vertex_array_object) {
        printf("ARB_vertex_array_object not available.\n");
    }

    glDisable(GL_CULL_FACE);

    assert(supportsOpenGL_2_0());
    assert(supportsOpenGL_3_0());
    assert(supportsOpenGL_3_3());
    assert(sizeof(GLfloat) == sizeof(float));
    assert(sizeof(GLdouble) == sizeof(double));

    return true;
}

GLenum toGL(gpu::ShaderType target) {
    switch (target) {
        case gpu::VERTEX_SHADER: return GL_VERTEX_SHADER;
        case gpu::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
        case gpu::GEOMETRY_SHADER: return GL_GEOMETRY_SHADER;
        default: assert("invalid"=="shader type");
    }
}

GLenum toGL(gpu::BufferType target) {
    switch (target) {
        case gpu::ARRAY_BUFFER: return GL_ARRAY_BUFFER;
        case gpu::INDEX_BUFFER: return GL_ELEMENT_ARRAY_BUFFER;
        default: assert("invalid"=="buffer type");
    }
    // should have asserted
    return GL_ARRAY_BUFFER;
}

GLenum toGL(gpu::TargetBuffer target) {
    GLenum result = 0;
    if (target & gpu::COLOR_BUFFER) {
        result |= GL_COLOR_BUFFER_BIT;
    }
    if (target & gpu::DEPTH_BUFFER) {
        result |= GL_DEPTH_BUFFER_BIT;
    }
    if (target & gpu::STENCIL_BUFFER) {
        result |= GL_STENCIL_BUFFER_BIT;
    }

    return result;
}

GLenum toGL(gpu::AttributeType type) {
    switch (type) {
        case gpu::INT: return GL_INT;
        case gpu::UNSIGNED_INT: return GL_UNSIGNED_INT;
        case gpu::FLOAT: return GL_FLOAT;
        case gpu::DOUBLE: return GL_DOUBLE;
    }
    // should have asserted
    return GL_FLOAT;
}

GLenum toGL(gpu::DrawMode mode) {
    switch (mode) {
        case gpu::TRIANGLES: return GL_TRIANGLES;
        case gpu::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case gpu::LINES: return GL_LINES;
        case gpu::LINE_STRIP: return GL_LINE_STRIP;
        case gpu::LINE_LOOP: return GL_LINE_LOOP;
    }
}

GLenum toGL(gpu::UpdateHint hint) {
    switch (hint) {
        case gpu::STATIC_UPDATE: return GL_STATIC_DRAW;
        case gpu::DYNAMIC_UPDATE: return GL_DYNAMIC_DRAW;
        case gpu::STREAM_UPDATE: return GL_STREAM_DRAW;
    }
}

static GLuint textureUnit(int i)
{
    switch(i)
    {
        case 0 : return GL_TEXTURE0;
        case 1 : return GL_TEXTURE1;
        case 2 : return GL_TEXTURE2;
        case 3 : return GL_TEXTURE3;
        case 4 : return GL_TEXTURE4;
        case 5 : return GL_TEXTURE5;
        case 6 : return GL_TEXTURE6;
        case 7 : return GL_TEXTURE7;
    }
    assert(false);
    return GL_TEXTURE0;
}

void RenderingContextGL::set_viewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
    CHECK_GL_ERROR
}

void RenderingContextGL::set_clear_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    CHECK_GL_ERROR
}

void RenderingContextGL::clear(TargetBuffer target)
{
    glClear(toGL(target));
    CHECK_GL_ERROR
}

void RenderingContextGL::upload(const gpu::VertexBuffer& vbo, Range<byte> data, UploadFlags flags)
{
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(byte), data.pointer(), toGL(vbo.update));
  CHECK_GL_ERROR
}

gpu::VertexArray RenderingContextGL::create_vertex_array()
{
    GLuint gl_vao = 0;
    glGenVertexArrays(1, &gl_vao);
    CHECK_GL_ERROR
    return VertexArray(gl_vao);
}

void RenderingContextGL::delete_vertex_array(gpu::VertexArray& vao)
{
    GLuint gl_vao = vao.handle;
    glDeleteVertexArrays(1, &gl_vao);
    vao.handle = 0;
    CHECK_GL_ERROR
}

void RenderingContextGL::bind(const VertexArray& vao)
{
    glBindVertexArray(vao.handle);
    CHECK_GL_ERROR
}

void RenderingContextGL::unbind(const VertexArray& vao)
{
    glBindVertexArray(0);
    CHECK_GL_ERROR
}

gpu::VertexBuffer RenderingContextGL::create_vertex_buffer()
{
    GLuint gl_vbo = 0;
    glGenBuffers(1, &gl_vbo);
    CHECK_GL_ERROR
    return VertexBuffer(gpu::STATIC_UPDATE, gl_vbo);
}

void RenderingContextGL::delete_vertex_buffer(gpu::VertexBuffer& vbo)
{
    GLuint gl_vbo = vbo.handle;
    glDeleteBuffers(1, &gl_vbo);
    CHECK_GL_ERROR
}

void RenderingContextGL::bind(const VertexBuffer& vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo.handle);
    CHECK_GL_ERROR
}

void RenderingContextGL::unbind(const VertexBuffer& vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR
}

void RenderingContextGL::define_vertex_attribute(int attribIndex,
                                               gpu::AttributeType type,
                                               int compPerVertex,
                                               int stride,
                                               int offset)
{
    // TODO: false stands for normalize which is in UploadFlags
    glVertexAttribPointer(attribIndex,
                          compPerVertex,
                          toGL(type),
                          false,
                          stride,
                          reinterpret_cast<void*>(offset));
    CHECK_GL_ERROR
}

void RenderingContextGL::enable_vertex_attribute(int index)
{
    glEnableVertexAttribArray(index);
    CHECK_GL_ERROR
}

void RenderingContextGL::disable_vertex_attribute(int index)
{
    glDisableVertexAttribArray(index);
    CHECK_GL_ERROR
}


gpu::Texture RenderingContextGL::create_texture(gpu::UpdateHint hint)
{
    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    CHECK_GL_ERROR
    glBindTexture(GL_TEXTURE_2D, gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return gpu::Texture(gl_tex);
}

void RenderingContextGL::delete_texture(gpu::Texture texture)
{
    GLuint gl_tex = (GLuint)texture.handle;
    glDeleteTextures(1, &gl_tex);
    texture.handle = 0;
    CHECK_GL_ERROR
}

void RenderingContextGL::bind(gpu::Texture texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.handle);
    CHECK_GL_ERROR
}

void RenderingContextGL::unbind(gpu::Texture tex)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR
}

void RenderingContextGL::set_texture_flags(gpu::Texture tex, TextureFlags flags)
{
    // TODO
    CHECK_GL_ERROR
}

void RenderingContextGL::upload(gpu::Texture dest, gfx::Surface* src, UploadFlags flags)
{
    printf("RenderingContextGL::upload %i, %i, %p\n",
                (int) src->get_size().width, (int) src->get_size().height, src->get_data());
    ScopedBind<Texture> bind(this, dest);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        src->get_size().width, src->get_size().height,
        0,
        GL_RGBA, GL_UNSIGNED_BYTE, src->get_data()
    );
    CHECK_GL_ERROR
}

gpu::Shader RenderingContextGL::create_shader(gpu::ShaderType type)
{
    Shader s(glCreateShader(toGL(type)));
    CHECK_GL_ERROR
    return s;
}

gpu::ShaderProgram RenderingContextGL::create_shaderProgram()
{
    ShaderProgram sp(glCreateProgram());
    CHECK_GL_ERROR
    return sp;
}

void RenderingContextGL::delete_shader(gpu::Shader shader)
{
    glDeleteShader(shader.handle);
    CHECK_GL_ERROR
}

bool RenderingContextGL::compile_shader(gpu::Shader shader, const ShaderSource& src)
{
    glShaderSource(shader.handle, src.count, src.str, src.len);
    CHECK_GL_ERROR
    glCompileShader(shader.handle);
    CHECK_GL_ERROR

    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    glGetShaderInfoLog(shader.handle, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        printf("Shader %i compilation - %s\n", (int)shader.handle, buffer);
    }

    GLint status;
    glGetShaderiv(shader.handle, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        printf("error while compiling shader\n");
        return false;
    }
    return true;
}

void RenderingContextGL::bind(gpu::ShaderProgram shader)
{
    glUseProgram(shader.handle);
    CHECK_GL_ERROR
}

void RenderingContextGL::unbind(gpu::ShaderProgram shader)
{
    glUseProgram(0);
    CHECK_GL_ERROR
}

void RenderingContextGL::attach_shader(gpu::ShaderProgram p, gpu::Shader s)
{
    glAttachShader(p.handle, s.handle);
    CHECK_GL_ERROR
}

bool RenderingContextGL::link_shader_program(gpu::ShaderProgram p)
{
    glLinkProgram(p.handle);
    CHECK_GL_ERROR
    // check for errors
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    glGetProgramInfoLog(p.handle, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        printf("Program %i link - %s\n", (int)p.handle, buffer);
    }

    glValidateProgram(p.handle);
    GLint status;
    glGetProgramiv(p.handle, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        printf("Error validating shader %i\n", (int)p.handle);
        return false;
    }
    return true;
}

bool RenderingContextGL::bind_attribute_location(gpu::ShaderProgram p,
                                               int location,
                                               const char* name)
{
    glBindAttribLocation(p.handle, location, name);
    CHECK_GL_ERROR
}

void RenderingContextGL::send_unirform(int location, float val)
{
    glUniform1f(location, val);
}
void RenderingContextGL::send_unirform(int location, const glm::vec2& val)
{
    glUniform2f(location, val[0], val[1]);
}
void RenderingContextGL::send_unirform(int location, const glm::vec3& val)
{
    glUniform3f(location, val[0], val[1], val[2]);
}
void RenderingContextGL::send_unirform(int location, const glm::vec4& val)
{
    glUniform4f(location, val[0], val[1], val[2], val[3]);
}
void RenderingContextGL::send_unirform(int location, const glm::mat4& val)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
}

void RenderingContextGL::send_unirform(int location,
                                     int texUnit,
                                     const Texture& tex)
{
    glActiveTexture(textureUnit(texUnit));
    glUniform1i(location, texUnit);
    bind(tex);
}


int RenderingContextGL::get_uniform_location(const ShaderProgram& p, const char* name)
{
    return glGetUniformLocation(p.handle, name);
}


void RenderingContextGL::draw_arrays(gpu::DrawMode mode, int first, int count)
{
    glDrawArrays(toGL(mode), first, count);
    CHECK_GL_ERROR
}


bool RenderingContextGL::is_supported(const char* str)
{
    return false;
    return glewIsSupported(str);
    CHECK_GL_ERROR
}

bool RenderingContextGL::is_supported(gpu::Feature feature)
{
    switch (feature) {
        case gpu::FRAGMENT_SHADING: return true;
        case gpu::VERTEX_SHADING: return true;
        case gpu::GEOMETRY_SHADING: return false;
        case gpu::RENDER_TO_TEXTURE: return true;
        case gpu::MULTIPLE_RENDER_TARGETS: return false;
    }
}

void RenderingContextGL::make_current()
{
    CHECK_GL_ERROR
}


} // namespace
} // namespace

#endif // VODK_GL_3
