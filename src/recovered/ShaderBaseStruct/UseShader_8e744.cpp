#include "class.h"

namespace AbyssEngine {

void ShaderBaseStruct::UseShader()
{
    glUseProgram(shader_program(this));
}

} // namespace AbyssEngine
