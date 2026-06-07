#include "class.h"

namespace AbyssEngine {

String ShaderBaseStruct::GetShaderName()
{
    return String(*shader_name(this), false);
}

} // namespace AbyssEngine
