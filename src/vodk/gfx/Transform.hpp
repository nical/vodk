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

#ifndef VODK_GFX_TRANSFORM_HPP
#define VODK_GFX_TRANSFORM_HPP

#include <glm/glm.hpp>

namespace vodk {
namespace gfx {

class Transform {
public:
    Transform();
    Transform(float aX, float aY, float aZ,
              float aSx = 1.0, float aSy = 1.0, float aSz = 1.0,
              float aRx = 0.0, float aRy = 0.0, float aRz = 0.0);

    float x;
    float y;
    float z;

    float sx;
    float sy;
    float sz;

    float rx;
    float ry;
    float rz;

    glm::mat4 matrix() const;
    glm::mat4 rotationMatrix() const;
    glm::mat4 translationMatrix() const;
    glm::mat4 scaleMatrix() const;
    glm::vec3 vec3() const;
    glm::vec4 vec4() const;

    void translate(float dx, float dy, float dz);
    void translate(const glm::vec3& offset);
};

} // gfx
} // vodk

#endif
