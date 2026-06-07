#include "class.h"

namespace AbyssEngine {

void FBOContainer::BeginCapture() {
    glBindFramebuffer(0x8d40, *(GLuint *)((char *)this + 0x0));
    glViewport(0, 0, *(GLsizei *)((char *)this + 0xc), *(GLsizei *)((char *)this + 0x10));
}

} // namespace AbyssEngine
