#include "class.h"

extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(void *self);
extern "C" void _ZN11AbyssEngine6StringC2EPKcb(AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(AbyssEngine::String *self, const AbyssEngine::String *other);
extern "C" void _ZN11AbyssEngine6StringD2Ev(AbyssEngine::String *self);

namespace AbyssEngine {

BumpMapping::BumpMapping()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev(this);
    *(uint32_t *)this = (uint32_t)_ZTVN11AbyssEngine11BumpMappingE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;

    char storage[sizeof(String)];
    String *name = (String *)storage;
    _ZN11AbyssEngine6StringC2EPKcb(name, "BumpMapping", false);
    _ZN11AbyssEngine6StringaSERKS0_((String *)((char *)this + 0xc), name);
    _ZN11AbyssEngine6StringD2Ev(name);
}

} // namespace AbyssEngine
