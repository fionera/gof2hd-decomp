#include "class.h"

extern "C" void _ZN11AbyssEngine18TextureLightShaderD0Ev(TextureLightShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
