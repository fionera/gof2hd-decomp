#include "class.h"

extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);

namespace AbyssEngine {

void BlurShader::SetInActive()
{
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x48));
    return glDisableVertexAttribArray_thunk(*(unsigned int *)((char *)this + 0x50));
}

} // namespace AbyssEngine
