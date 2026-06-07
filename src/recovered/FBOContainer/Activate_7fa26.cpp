#include "class.h"

namespace AbyssEngine {

void FBOContainer::Activate() {
    glBindTexture(0xde1, *(GLuint *)((char *)this + 0x4));
}

} // namespace AbyssEngine
