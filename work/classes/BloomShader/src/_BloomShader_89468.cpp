#include "class.h"

extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
extern "C" char BloomShader_vtable;

extern "C" void *_ZN11AbyssEngine11BloomShaderD1Ev(BloomShader *self)
{
    *(void **)self = &BloomShader_vtable + 8;
    return ShaderBaseStruct_dtor((ShaderBaseStruct *)self);
}
