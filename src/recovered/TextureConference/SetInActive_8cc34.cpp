#include "class.h"

namespace AbyssEngine {

void TextureConference::SetInActive()
{
    glDisableVertexAttribArray(field_i32(this, 0x20));
    return glDisableVertexAttribArray(field_i32(this, 0x24));
}

} // namespace AbyssEngine
