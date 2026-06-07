#include "class.h"

extern "C" void _ZN11AbyssEngine13GenericShaderD0Ev(GenericShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
