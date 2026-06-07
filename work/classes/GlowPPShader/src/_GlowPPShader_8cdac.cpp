#include "class.h"

extern "C" void _ZN11AbyssEngine12GlowPPShaderD0Ev(GlowPPShader *self)
{
    operator delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}
