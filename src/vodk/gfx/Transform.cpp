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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vodk/gfx/Transform.hpp"

namespace vodk {
namespace gfx {

Transform::Transform()
: x(0.0), y(0.0), z(0.0)
, rx(0.0), ry(0.0), rz(0.0)
, sx(1.0), sy(1.0), sz(1.0)
{}

Transform::Transform(float aX,  float aY,  float aZ,
                     float aSx, float aSy, float aSz,
                     float aRx, float aRy, float aRz)
: x(aX), y(aY), z(aZ)
, rx(aRx), ry(aRy), rz(aRz)
, sx(aSx), sy(aSy), sz(aSz)
{}

glm::mat4 Transform::matrix() const
{
    glm::mat4 result(1.0);
    glm::translate(result, vec3());
    glm::rotate(result, rz, glm::vec3(0.0,0.0,1.0));
    glm::scale(result, glm::vec3(sx, sy, sz));
    return result;
}

glm::vec3 Transform::vec3() const
{
    return glm::vec3(x, y, z);
}

glm::vec4 Transform::vec4() const
{
    return glm::vec4(x, y, z, 0.0);
}

void Transform::translate(float dx, float dy, float dz)
{
    x += dx;
    y += dy;
    z += dz;
}

void Transform::translate(const glm::vec3& offset)
{
    x += offset.x;
    y += offset.y;
    z += offset.z;
}

} // gfx
} // vodk
