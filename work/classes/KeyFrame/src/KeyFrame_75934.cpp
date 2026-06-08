#include "class.h"

namespace AbyssEngine {

// AbyssEngine::KeyFrame::KeyFrame()
//   Clears the leading 0x48 bytes, seeds the two embedded vectors to (1,1,1),
//   sets the +0x48 weight to 1.0, and zeroes the four trailing words.
KeyFrame::KeyFrame()
{
    u32(this, 0x50) = 0;
    u32(this, 0x54) = 0;
    __aeabi_memclr4(this, 0x48);

    AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    *(AEMath::Vector *)((char *)this + 0xc) = one;
    *(AEMath::Vector *)((char *)this + 0x30) = one;

    u32(this, 0x58) = 0;
    u32(this, 0x5c) = 0;
    f32(this, 0x48) = 1.0f;
}

} // namespace AbyssEngine
