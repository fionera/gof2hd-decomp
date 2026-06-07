#include "class.h"

extern "C" void _ZN11AbyssEngine13TextureShaderD0Ev(TextureShader *self)
{
    operator delete(ShaderBaseStruct_dtor(self));
}
