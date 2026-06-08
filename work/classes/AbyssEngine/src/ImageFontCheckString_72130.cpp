#include "class.h"

// AbyssEngine::ImageFontCheckString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int)
// The shipped body just spins a counter to param_3 (a stubbed/optimized-out validator).
namespace AbyssEngine {

void ImageFontCheckString(ImageFont * /*font*/, const unsigned short * /*str*/, unsigned int count)
{
    unsigned short i = 0;
    unsigned int v;
    do {
        v = (unsigned int)i;
        i = i + 1;
    } while (v < count);
}

} // namespace AbyssEngine
