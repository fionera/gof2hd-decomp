#include "class.h"

extern "C" void GlowPPShader_SetInActive(GlowPPShader *self)
{
    glDisableVertexAttribArray(field_u32(self, 0x90));
    return glDisableVertexAttribArray(field_u32(self, 0x98));
}
