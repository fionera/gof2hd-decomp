#include "class.h"

extern "C" void _ZN11AbyssEngine12SimpleShaderD0Ev(SimpleShader *self)
{
    operator delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)self));
}
