#include "class.h"

namespace AbyssEngine {

void FBOContainer::Create(int width, int height, bool a, bool linear) {
    *(int *)((char *)this + 0xc) = width;
    *(int *)((char *)this + 0x10) = height;
    glGenFramebuffers(1, (GLuint *)((char *)this + 0x0));
    glBindFramebuffer(0x8d40, *(GLuint *)((char *)this + 0x0));
    glGenTextures(1, (GLuint *)((char *)this + 0x4));
    glBindTexture(0xde1, *(GLuint *)((char *)this + 0x4));
    glPixelStorei(0xcf5, 1);
    glTexParameteri(0xde1, 0x2802, 0x812f);
    glTexParameteri(0xde1, 0x2803, 0x812f);
    GLint filter;
    if (linear) {
        glTexParameteri(0xde1, 0x2800, 0x2601);
        filter = 0x2601;
    } else {
        glTexParameteri(0xde1, 0x2800, 0x2600);
        filter = 0x2600;
    }
    glTexParameteri(0xde1, 0x2801, filter);
    glTexImage2D(0xde1, 0, 0x1908, *(GLsizei *)((char *)this + 0xc),
                 *(GLsizei *)((char *)this + 0x10), 0, 0x1908, 0x1401, 0);
    glFramebufferTexture2D(0x8d40, 0x8ce0, 0xde1, *(GLuint *)((char *)this + 0x4), 0);
    glGenRenderbuffers(1, (GLuint *)((char *)this + 0x8));
    glBindRenderbuffer(0x8d41, *(GLuint *)((char *)this + 0x8));
    glRenderbufferStorage(0x8d41, 0x81a5, *(GLsizei *)((char *)this + 0xc),
                          *(GLsizei *)((char *)this + 0x10));
    glFramebufferRenderbuffer(0x8d40, 0x8d00, 0x8d41, *(GLuint *)((char *)this + 0x8));
    glCheckFramebufferStatus(0x8d40);
    *(unsigned char *)((char *)this + 0x18) = 1;
    Engine *engine = *(Engine **)((char *)this + 0x14);
    glBindFramebuffer(0x8d40, *(GLuint *)((char *)engine + 0x40c));
}

} // namespace AbyssEngine
