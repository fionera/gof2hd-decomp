#include "class.h"

extern "C" void _ZN11AbyssEngine12TexOnlyShaderD0Ev(TexOnlyShader *self)
{
    operator delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}
