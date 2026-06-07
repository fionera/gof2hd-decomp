#include "class.h"

extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine21TextureVtxColorShaderD0Ev(
    AbyssEngine::TextureVtxColorShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}
