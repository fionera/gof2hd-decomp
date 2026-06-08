#include "class.h"

namespace AbyssEngine {

// AbyssEngine::KeyFrame::KeyFrame()
//   Clears the leading 0x48 bytes, seeds the two embedded vectors to (1,1,1),
//   sets the +0x48 weight to 1.0, and zeroes the four trailing words.
KeyFrame::KeyFrame()
{
    this->f_50 = 0;
    this->f_54 = 0;
    __aeabi_memclr4(this, 0x48);

    AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    *(AEMath::Vector *)((char *)this + 0xc) = one;
    *(AEMath::Vector *)((char *)this + 0x30) = one;

    this->f_58 = 0;
    this->f_5c = 0;
    this->f_48 = 1.0f;
}

} // namespace AbyssEngine
