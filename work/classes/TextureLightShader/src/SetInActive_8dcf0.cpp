#include "class.h"

namespace AbyssEngine {

void TextureLightShader::SetInActive()
{
    glDisableVertexAttribArray(i32(this, 0x20));
    glDisableVertexAttribArray(i32(this, 0x24));
    return glDisableVertexAttribArray(i32(this, 0x28));
}

} // namespace AbyssEngine
