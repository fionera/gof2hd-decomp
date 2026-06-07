#include "class.h"

namespace AbyssEngine {

void FBOContainer::Release() {
    if (*(unsigned char *)((char *)this + 0x18) == 0) {
        return;
    }
    glDeleteFramebuffers(1, (GLuint *)((char *)this + 0x0));
    *(GLuint *)((char *)this + 0x0) = 0;
    glDeleteTextures(1, (GLuint *)((char *)this + 0x4));
    *(GLuint *)((char *)this + 0x4) = 0;
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x8));
    *(GLuint *)((char *)this + 0x8) = 0;
    *(unsigned char *)((char *)this + 0x18) = 0;
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x30));
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x34));
    glBindFramebuffer(0x8d40, 0);
    return glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x2c));
}

} // namespace AbyssEngine
