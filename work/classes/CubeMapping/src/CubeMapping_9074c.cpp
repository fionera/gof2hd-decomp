#include "class.h"

namespace AbyssEngine {

CubeMapping::CubeMapping()
{
    ShaderBaseStruct_ctor(this);
    *(int *)this = (int)(CubeMapping_vtable) + 8;
    CubeMapping_typeInfoDest = CubeMapping_typeInfoSource;
    String tmp(CubeMapping_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine
