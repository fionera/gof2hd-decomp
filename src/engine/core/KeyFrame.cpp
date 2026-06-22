#include "engine/core/KeyFrame.h"

namespace AbyssEngine {
    KeyFrame::KeyFrame() {
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
