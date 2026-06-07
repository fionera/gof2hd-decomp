#include "class.h"

extern "C" void _ZN11AbyssEngine10BumpShaderD0Ev(BumpShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
