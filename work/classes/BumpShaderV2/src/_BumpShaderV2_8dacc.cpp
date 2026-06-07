#include "class.h"

extern "C" void _ZN11AbyssEngine12BumpShaderV2D0Ev(BumpShaderV2 *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
