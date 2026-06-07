#include "class.h"

namespace AbyssEngine {

void ShaderBaseStruct::Update()
{
    shader_dirty(this) = 1;
}

} // namespace AbyssEngine
