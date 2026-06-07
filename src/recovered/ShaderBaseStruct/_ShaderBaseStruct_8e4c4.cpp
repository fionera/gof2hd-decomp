#include "class.h"

namespace AbyssEngine {

ShaderBaseStruct::~ShaderBaseStruct()
{
    shader_vtable(this) = (uint32_t)ShaderBaseStruct_vtable + 8;
    shader_name(this)->~String();
}

} // namespace AbyssEngine
