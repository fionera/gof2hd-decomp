#include "class.h"

extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glDisableVertexAttribArray_thunk(unsigned int index);

namespace AbyssEngine {

void BloomShader::SetInActive()
{
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x8c));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x94));
    glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x2c));
    return glDisableVertexAttribArray(*(unsigned int *)((char *)this + 0x24));
}

} // namespace AbyssEngine
