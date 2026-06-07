#include "class.h"

extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, int width, int height);

extern "C" void Engine_ActivateViewBuffer(Engine *self)
{
    glBindFramebuffer(0x8d40, *(uint32_t *)((char *)self + 0x40c));
    return glViewport(0, 0, *(int *)((char *)self + 0x370), *(int *)((char *)self + 0x374));
}
