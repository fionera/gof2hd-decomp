#include "class.h"

extern "C" void _ZN11AbyssEngine11CubeMappingD0Ev(CubeMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}
