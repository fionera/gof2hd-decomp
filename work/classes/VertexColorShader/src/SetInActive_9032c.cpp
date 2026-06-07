#include "class.h"

extern "C" void glDisableVertexAttribArray(unsigned int index);

extern "C" void VertexColorShader_SetInActive(AbyssEngine::VertexColorShader *self)
{
    if (ae_i32(self, 0x20) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x20));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x24));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x28));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x2c));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x30));
    }
    if (ae_i32(self, 0x34) >= 0) {
        return glDisableVertexAttribArray(ae_u32(self, 0x34));
    }
}
