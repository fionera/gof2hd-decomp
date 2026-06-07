#include "class.h"

namespace AbyssEngine {

void ColorMixAdd::SetInActive()
{
    for (int i = 2; i != 0; i = i - 1) {
        glDisableVertexAttribArray(field_i32(this, 0x20));
        glDisableVertexAttribArray(field_i32(this, 0x24));
    }
}

} // namespace AbyssEngine
