#include "class.h"

// AbyssEngine::ImageFontSetYOffset(AbyssEngine::ImageFont*, short)
namespace AbyssEngine {

void ImageFontSetYOffset(ImageFont *font, short yOffset)
{
    if (font != 0)
        s16(font, 0x12) = yOffset;
}

} // namespace AbyssEngine
