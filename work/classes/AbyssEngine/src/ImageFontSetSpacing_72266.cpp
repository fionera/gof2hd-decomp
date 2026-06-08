#include "class.h"

// AbyssEngine::ImageFontSetSpacing(AbyssEngine::ImageFont*, short)
namespace AbyssEngine {

void ImageFontSetSpacing(ImageFont *font, short spacing)
{
    if (font != 0)
        s16(font, 0x10) = spacing;
}

} // namespace AbyssEngine
