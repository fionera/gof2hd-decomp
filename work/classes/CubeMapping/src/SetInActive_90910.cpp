#include "class.h"

namespace AbyssEngine {

void CubeMapping::SetInActive()
{
    glDisableVertexAttribArray(i32(this, 0x20));
    glDisableVertexAttribArray(i32(this, 0x24));
    return glDisableVertexAttribArray(i32(this, 0x28));
}

} // namespace AbyssEngine
