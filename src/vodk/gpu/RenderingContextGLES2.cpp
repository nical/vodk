#include "vodk/core/config.hpp"

#ifdef VODK_GL_ES2

#include "vodk/gpu/RenderingContext.hpp"
#include "vodk/gpu/ScopedBindObject.hpp"
#include "vodk/gfx/Surface.hpp"

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <memory.h>

#define CHECK_GL_ERROR check_gl_error(__LINE__,__PRETTY_FUNCTION__);

namespace vodk {

namespace io {
class Window;
}

namespace gpu {

class RenderingContextGLES2 : public RenderingContext {
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

static const char* glErrorToStr(GLenum err) {
    switch (err) {
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
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


RenderingContextGLES2* createRenderingContext(io::Window* w) {
    printf(" -- Creating RenderingContext GLES2\n");
    return new RenderingContextGLES2();
}

void deleteRenderingContext(RenderingContext* ctx) {
    delete ctx;
}

bool initBackend() {
    glDisable(GL_CULL_FACE);

    assert(sizeof(GLfloat) == sizeof(float));

    return true;
}

static GLenum toGL(gpu::ShaderType target) {
    switch (target) {
        case gpu::VERTEX_SHADER: return GL_VERTEX_SHADER;
        case gpu::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
        default: assert("invalid"=="shader type");
    }
}

static GLenum toGL(gpu::BufferType target) {
    switch (target) {
        case gpu::ARRAY_BUFFER: return GL_ARRAY_BUFFER;
        case gpu::INDEX_BUFFER: return GL_ELEMENT_ARRAY_BUFFER;
        default: assert("invalid"=="buffer type");
    }
    // should have asserted
    return GL_ARRAY_BUFFER;
}

static GLenum toGL(gpu::TargetBuffer target) {
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

static GLenum toGL(gpu::AttributeType type) {
    switch (type) {
        case gpu::INT: return GL_INT;
        case gpu::UNSIGNED_INT: return GL_UNSIGNED_INT;
        case gpu::FLOAT: return GL_FLOAT;
    }
    // should have asserted
    return GL_FLOAT;
}

static GLenum toGL(gpu::DrawMode mode) {
    switch (mode) {
        case gpu::TRIANGLES: return GL_TRIANGLES;
        case gpu::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case gpu::LINES: return GL_LINES;
        case gpu::LINE_STRIP: return GL_LINE_STRIP;
        case gpu::LINE_LOOP: return GL_LINE_LOOP;
    }
}

static GLenum toGL(gpu::UpdateHint hint) {
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

void RenderingContextGLES2::set_viewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::set_clear_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::clear(TargetBuffer target)
{
    glClear(toGL(target));
    CHECK_GL_ERROR
}

void RenderingContextGLES2::upload(const gpu::VertexBuffer& vbo, Range<byte> data, UploadFlags flags)
{
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(byte), data.pointer(), toGL(vbo.update));
  CHECK_GL_ERROR
}

gpu::VertexArray RenderingContextGLES2::create_vertex_array()
{
    // TODO: VAO not supported in GLES2
    return VertexArray(0);
}

void RenderingContextGLES2::delete_vertex_array(gpu::VertexArray& vao)
{
    // TODO: VAO not supported in GLES2
}

void RenderingContextGLES2::bind(const VertexArray& vao)
{
    // TODO: VAO not supported in GLES2
}

void RenderingContextGLES2::unbind(const VertexArray& vao)
{
    // TODO: VAO not supported in GLES2
}

gpu::VertexBuffer RenderingContextGLES2::create_vertex_buffer()
{
    GLuint gl_vbo = 0;
    glGenBuffers(1, &gl_vbo);
    CHECK_GL_ERROR
    return VertexBuffer(gpu::STATIC_UPDATE, gl_vbo);
}

void RenderingContextGLES2::delete_vertex_buffer(gpu::VertexBuffer& vbo)
{
    GLuint gl_vbo = vbo.handle;
    glDeleteBuffers(1, &gl_vbo);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::bind(const VertexBuffer& vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo.handle);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::unbind(const VertexBuffer& vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::define_vertex_attribute(int attribIndex,
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

void RenderingContextGLES2::enable_vertex_attribute(int index)
{
    glEnableVertexAttribArray(index);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::disable_vertex_attribute(int index)
{
    glDisableVertexAttribArray(index);
    CHECK_GL_ERROR
}


gpu::Texture RenderingContextGLES2::create_texture(gpu::UpdateHint hint)
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

void RenderingContextGLES2::delete_texture(gpu::Texture texture)
{
    GLuint gl_tex = (GLuint)texture.handle;
    glDeleteTextures(1, &gl_tex);
    texture.handle = 0;
    CHECK_GL_ERROR
}

void RenderingContextGLES2::bind(gpu::Texture texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.handle);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::unbind(gpu::Texture tex)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::set_texture_flags(gpu::Texture tex, TextureFlags flags)
{
    // TODO
    CHECK_GL_ERROR
}

void RenderingContextGLES2::upload(gpu::Texture dest, gfx::Surface* src, UploadFlags flags)
{
    ScopedBind<Texture> bind(this, dest);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        src->get_size().width, src->get_size().height,
        0,
        GL_RGBA, GL_UNSIGNED_BYTE, src->get_data()
    );
    CHECK_GL_ERROR
}

gpu::Shader RenderingContextGLES2::create_shader(gpu::ShaderType type)
{
    Shader s(glCreateShader(toGL(type)));
    CHECK_GL_ERROR
    return s;
}

gpu::ShaderProgram RenderingContextGLES2::create_shaderProgram()
{
    ShaderProgram sp(glCreateProgram());
    CHECK_GL_ERROR
    return sp;
}

void RenderingContextGLES2::delete_shader(gpu::Shader shader)
{
    glDeleteShader(shader.handle);
    CHECK_GL_ERROR
}

bool RenderingContextGLES2::compile_shader(gpu::Shader shader, const ShaderSource& src)
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

void RenderingContextGLES2::bind(gpu::ShaderProgram shader)
{
    glUseProgram(shader.handle);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::unbind(gpu::ShaderProgram shader)
{
    glUseProgram(0);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::attach_shader(gpu::ShaderProgram p, gpu::Shader s)
{
    glAttachShader(p.handle, s.handle);
    CHECK_GL_ERROR
}

bool RenderingContextGLES2::link_shader_program(gpu::ShaderProgram p)
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

bool RenderingContextGLES2::bind_attribute_location(gpu::ShaderProgram p,
                                               int location,
                                               const char* name)
{
    glBindAttribLocation(p.handle, location, name);
    CHECK_GL_ERROR
}

void RenderingContextGLES2::send_unirform(int location, float val)
{
    glUniform1f(location, val);
}
void RenderingContextGLES2::send_unirform(int location, const glm::vec2& val)
{
    glUniform2f(location, val[0], val[1]);
}
void RenderingContextGLES2::send_unirform(int location, const glm::vec3& val)
{
    glUniform3f(location, val[0], val[1], val[2]);
}
void RenderingContextGLES2::send_unirform(int location, const glm::vec4& val)
{
    glUniform4f(location, val[0], val[1], val[2], val[3]);
}
void RenderingContextGLES2::send_unirform(int location, const glm::mat4& val)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
}

void RenderingContextGLES2::send_unirform(int location,
                                     int texUnit,
                                     const Texture& tex)
{
    glActiveTexture(textureUnit(texUnit));
    glUniform1i(location, texUnit);
    bind(tex);
}


int RenderingContextGLES2::get_uniform_location(const ShaderProgram& p, const char* name)
{
    return glGetUniformLocation(p.handle, name);
}


void RenderingContextGLES2::draw_arrays(gpu::DrawMode mode, int first, int count)
{
    glDrawArrays(toGL(mode), first, count);
    CHECK_GL_ERROR
}


bool RenderingContextGLES2::is_supported(const char* str)
{
    return false;
//    return glewIsSupported(str);
//    CHECK_GL_ERROR
}

bool RenderingContextGLES2::is_supported(gpu::Feature feature)
{
    switch (feature) {
        case gpu::FRAGMENT_SHADING: return true;
        case gpu::VERTEX_SHADING: return true;
        case gpu::GEOMETRY_SHADING: return false;
        case gpu::RENDER_TO_TEXTURE: return true;
        case gpu::MULTIPLE_RENDER_TARGETS: return false;
    }
}

void RenderingContextGLES2::make_current()
{
    CHECK_GL_ERROR
}


} // namespace
} // namespace

#endif // VODK_GL_ES2
