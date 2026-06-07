#include "class.h"

namespace AbyssEngine {

void NoTexShader::SetInActive()
{
    return glDisableVertexAttribArray(field_i32(this, 0x20));
}

} // namespace AbyssEngine
