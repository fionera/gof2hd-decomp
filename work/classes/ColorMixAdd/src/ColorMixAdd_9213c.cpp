#include "class.h"

extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

int ColorMixAdd::ShaderIndex;

ColorMixAdd::ColorMixAdd()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine11ColorMixAddE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    String tmp("ColorMixAdd", false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine
