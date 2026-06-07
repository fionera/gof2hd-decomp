#include "class.h"

namespace AbyssEngine {

void PostBWShader::SetInActive()
{
    glDisableVertexAttribArray(field_u32(this, 0x20));
    return glDisableVertexAttribArray(field_u32(this, 0x28));
}

} // namespace AbyssEngine
