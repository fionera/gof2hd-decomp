#include "class.h"

extern "C" {
void _ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
extern void *_ZTIN11AbyssEngine17CubeNormalMappingE;
extern void *CubeNormalMapping_typeInfoSlot;
extern const char CubeNormalMapping_name[];
}

namespace AbyssEngine {

CubeNormalMapping::CubeNormalMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);
    *(int *)this = (int)(_ZTVN11AbyssEngine17CubeNormalMappingE) + 8;
    CubeNormalMapping_typeInfoSlot = &_ZTIN11AbyssEngine17CubeNormalMappingE;
    String tmp(CubeNormalMapping_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine
