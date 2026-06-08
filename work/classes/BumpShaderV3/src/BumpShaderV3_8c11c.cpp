#include "class.h"

// Engine symbols used by the constructor (resolved at link time).
extern "C" {
// AbyssEngine::ShaderBaseStruct::ShaderBaseStruct(this) -- 0x00070930.
void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(void *self);
// AbyssEngine::String::String(char const*, bool) / operator= / ~String.
void _ZN11AbyssEngine6StringC2EPKcb(AbyssEngine::String *self, const char *text, bool copy);
AbyssEngine::String &_ZN11AbyssEngine6StringaSERKS0_(AbyssEngine::String *self,
                                                     const AbyssEngine::String *other);
void _ZN11AbyssEngine6StringD2Ev(AbyssEngine::String *self);

// BumpShaderV3 vtable; the ctor stores (&vtable + 8) into this[0].
extern char _ZTVN11AbyssEngine12BumpShaderV3E[];
// Two statics related by the engine: ShaderBaseStruct::shaderIndexIntern = BumpShaderV3::ShaderIndex.
extern int _ZN11AbyssEngine12BumpShaderV311ShaderIndexE;
extern int _ZN11AbyssEngine16ShaderBaseStruct16shaderIndexInternE;
}

namespace AbyssEngine {

// AbyssEngine::BumpShaderV3::BumpShaderV3()
BumpShaderV3::BumpShaderV3()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev(this);

    // install vtable (+8 past the RTTI/offset slots) and copy the shader-index static.
    *(void **)this = (void *)(_ZTVN11AbyssEngine12BumpShaderV3E + 8);
    _ZN11AbyssEngine16ShaderBaseStruct16shaderIndexInternE =
        _ZN11AbyssEngine12BumpShaderV311ShaderIndexE;

    // name = String("BumpShaderV3"); this->name(0xc) = name; ~name.
    String name;
    _ZN11AbyssEngine6StringC2EPKcb(&name, "BumpShaderV3", false);
    _ZN11AbyssEngine6StringaSERKS0_((String *)((char *)this + 0xc), &name);
    _ZN11AbyssEngine6StringD2Ev(&name);
}

} // namespace AbyssEngine
