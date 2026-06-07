#include "class.h"

extern "C" void _ZN11AbyssEngine20SpecCubeAlphaMappingD0Ev(SpecCubeAlphaMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
