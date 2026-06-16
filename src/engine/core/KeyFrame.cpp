#include "engine/core/KeyFrame.h"

namespace AbyssEngine {

// Seeds the two transform vectors to (1,1,1), the blend weight to 1.0,
// and zeroes the trailing words and padding.
KeyFrame::KeyFrame()
{
    for (unsigned char& b : _pad_00) b = 0;
    for (unsigned char& b : _pad_18) b = 0;
    for (unsigned char& b : _pad_3c) b = 0;
    for (unsigned char& b : _pad_4c) b = 0;

    const AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    scaleA = one;
    scaleB = one;
    flWeight = 1.0f;

    field_0x50 = 0;
    field_0x54 = 0;
    field_0x58 = 0;
    field_0x5c = 0;
}

} // namespace AbyssEngine
