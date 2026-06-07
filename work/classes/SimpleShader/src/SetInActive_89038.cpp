#include "class.h"

namespace AbyssEngine {

void SimpleShader::SetInActive()
{
    return glDisableVertexAttribArray(field_u32(this, 0x20));
}

} // namespace AbyssEngine
