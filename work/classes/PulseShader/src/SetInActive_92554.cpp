#include "class.h"

namespace AbyssEngine {

void PulseShader::SetInActive()
{
    if (i32(this, 0x20) >= 0)
        glDisableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glDisableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glDisableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glDisableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glDisableVertexAttribArray(i32(this, 0x30));
}

} // namespace AbyssEngine
