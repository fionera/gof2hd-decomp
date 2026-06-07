#include "class.h"

namespace AbyssEngine {

void ShaderBaseStruct::DeleteShader()
{
    glDeleteProgram(shader_program(this));
}

} // namespace AbyssEngine
