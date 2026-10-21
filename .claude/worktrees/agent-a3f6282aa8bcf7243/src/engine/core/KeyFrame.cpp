#include "engine/core/KeyFrame.h"

namespace AbyssEngine {

// Clears the whole transform block (translation..localRotation) in one shot,
// then seeds both scale channels to (1,1,1) and the blend weight to 1.0. The
// timestamp words and the two trailing words are zeroed; +0x4c is left alone.
KeyFrame::KeyFrame()
{
    const AEMath::Vector one = {1.0f, 1.0f, 1.0f};

    timestampLo = 0;
    timestampHi = 0;
    __builtin_memset(this, 0, 0x48);
    scale = one;
    localScale = one;
    field_0x58 = 0;
    field_0x5c = 0;
    alpha = 1.0f;
}

} // namespace AbyssEngine
