#include "class.h"

// AbyssEngine::ImageFontGetSpacing(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetSpacing(ImageFont *font)
{
    short v = (font == 0) ? (short)0 : s16(font, 0x10);
    return (int)v;
}

} // namespace AbyssEngine
