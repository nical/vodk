
#ifndef VODK_GFX_VECTORIZER_HPP
#define VODK_GFX_VECTORIZER_HPP

#include <vector>
#include <glm/glm.hpp>

namespace vodk {
namespace gfx {

class ImageSurface;

bool vectorize(ImageSurface* input, std::vector<glm::vec2>& output);

} // gfx
} // vodk

#endif
