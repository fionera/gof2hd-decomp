#include "class.h"

namespace AbyssEngine {

void BumpMapping::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    disableVertexAttribArray(field_i32(this, 0x20));
    disableVertexAttribArray(field_i32(this, 0x24));
    disableVertexAttribArray(field_i32(this, 0x28));
    disableVertexAttribArray(field_i32(this, 0x2c));
    disableVertexAttribArray(field_i32(this, 0x30));
}

} // namespace AbyssEngine
