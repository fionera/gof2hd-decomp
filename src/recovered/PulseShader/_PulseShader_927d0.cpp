#include "class.h"

extern "C" void _ZN11AbyssEngine11PulseShaderD0Ev(PulseShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
