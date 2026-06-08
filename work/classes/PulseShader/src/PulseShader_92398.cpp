#include "class.h"

namespace AbyssEngine {

PulseShader::PulseShader()
{
    ShaderBaseStruct_ctor(this);
    *(int *)this = (int)(PulseShader_vtable) + 8;
    PulseShader_typeInfoDest = PulseShader_typeInfoSource;
    String tmp(PulseShader_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine
