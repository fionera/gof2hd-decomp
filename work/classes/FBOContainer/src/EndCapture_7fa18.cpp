#include "class.h"

namespace AbyssEngine {

void FBOContainer::EndCapture() {
    Engine *engine = *(Engine **)((char *)this + 0x14);
    glBindFramebuffer(0x8d40, *(GLuint *)((char *)engine + 0x40c));
}

} // namespace AbyssEngine
