#include "class.h"

// AbyssEngine::ImageFontGetYOffset(AbyssEngine::ImageFont*)
namespace AbyssEngine {

int ImageFontGetYOffset(ImageFont *font)
{
    short v = (font == 0) ? (short)0 : s16(font, 0x12);
    return (int)v;
}

} // namespace AbyssEngine
