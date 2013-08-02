
#include "vodk/gfx/vectorizer.hpp"
#include "vodk/gfx/ImageSurface.hpp"

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>

namespace vodk {
namespace gfx {

enum Channel {
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ALPHA = 3
};

uint8_t* get(ImageSurface* surf, int x, int y, Channel channel = ALPHA) {
	return surf->get_data()
		+ (y * surf->get_stride() + x )
			* bytes_per_pixel(surf->get_format())
		+ channel;
}

bool vectorize(ImageSurface* input, std::vector<glm::vec2>& output) {
    printf("vecotrize: w:%i h:%i stride:%i fmt:%i\n",
    	   input->get_size().width, input->get_size().height,
    	   input->get_stride(), input->get_format());

    int w = input->get_size().width;
    int h = input->get_size().height;
    int ix = 0;
    int iy = input->get_size().width/2;

    uint8_t* data = input->get_data();
    int stride = input->get_stride();

    // find a point along the edge
    bool hit = false;
    while (!hit) {
	    while (ix != w) {
	    	printf("read pixel (%i, %i) %i\n", ix, iy, (int)*get(input, ix, iy));
	    	if (*get(input, ix, iy) > 100) {
	    		hit = true;
	    		break;
	    	}
	    	++ix;
	    }
	    if (hit) break;
	    iy += 1;
	    if (iy > w) {
	    	iy = 0;
	    }
	    if (iy == input->get_size().width/2) {
	    	return nullptr;
	    }
    }

    *get(input, ix, iy, BLUE) = 255;
    *get(input, ix, iy, GREEN) = 255;
    *get(input, ix, iy, ALPHA) = 255;

    printf("vecotrize: %i %i\n", ix, iy);

    return false;
}

} // vodk
} // gfx
