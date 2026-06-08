#include "class.h"

namespace AbyssEngine {

// AbyssEngine::FBOContainer::FBOContainer(AbyssEngine::Engine*, AbyssEngine::String)
FBOContainer::FBOContainer(Engine *engine, String name)
{
    char *self = (char *)this;

    // Construct the embedded String at +0x1c.
    new ((void *)(self + 0x1c)) String();

    *(unsigned char *)(self + 0x18) = 0;
    *(unsigned char *)(self + 0x28) = 0;

    // Zero framebuffer / texture / renderbuffer / width and clear height.
    *(int *)(self + 0x0) = 0;
    *(int *)(self + 0x4) = 0;
    *(int *)(self + 0x8) = 0;
    *(int *)(self + 0xc) = 0;
    *(int *)(self + 0x10) = 0;

    // Copy the supplied name into the embedded String.
    *(String *)(self + 0x1c) = name;

    *(Engine **)(self + 0x14) = engine;
}

} // namespace AbyssEngine
