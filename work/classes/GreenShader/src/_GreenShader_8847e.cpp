#include "class.h"

extern "C" void _ZN11AbyssEngine11GreenShaderD0Ev(GreenShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
