#include "class.h"

namespace AbyssEngine {

void NoTexVtxColorShader::SetInActive()
{
    glDisableVertexAttribArray(field_i32(this, 0x20));
    return glDisableVertexAttribArray(field_i32(this, 0x24));
}

} // namespace AbyssEngine
