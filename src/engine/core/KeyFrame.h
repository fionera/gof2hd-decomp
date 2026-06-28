#ifndef GOF2_KEYFRAME_H
#define GOF2_KEYFRAME_H

#include "engine/core/Array.h"
#include "engine/math/Vector.h"

#include "engine/math/AEMath.h"

namespace AbyssEngine {
    class KeyFrame {
    public:
        AEMath::Vector translation;
        AEMath::Vector scale;
        AEMath::Vector rotation;
        AEMath::Vector localTranslation;
        AEMath::Vector localScale;
        AEMath::Vector localRotation;
        float alpha;
        uint32_t field_0x4c;
        int64_t timestamp;
        uint32_t channelFlags;
        uint32_t channelFlagsHi;

        // Defined inline so the complete-object ctor (C1) folds into the base-object C2 the
        // original exports (see docs/VALIDATION.md).
        KeyFrame() {
            const AEMath::Vector one = {1.0f, 1.0f, 1.0f};

            timestamp = 0;
            __builtin_memset(this, 0, 0x48);
            scale = one;
            localScale = one;
            channelFlags = 0;
            channelFlagsHi = 0;
            alpha = 1.0f;
        }
    };

#if defined(GOF2_MATCH) && __SIZEOF_POINTER__ == 4
    static_assert(__builtin_offsetof(KeyFrame, translation) == 0x00, "KeyFrame::translation");
    static_assert(__builtin_offsetof(KeyFrame, scale) == 0x0c, "KeyFrame::scale");
    static_assert(__builtin_offsetof(KeyFrame, rotation) == 0x18, "KeyFrame::rotation");
    static_assert(__builtin_offsetof(KeyFrame, localTranslation) == 0x24, "KeyFrame::localTranslation");
    static_assert(__builtin_offsetof(KeyFrame, localScale) == 0x30, "KeyFrame::localScale");
    static_assert(__builtin_offsetof(KeyFrame, localRotation) == 0x3c, "KeyFrame::localRotation");
    static_assert(__builtin_offsetof(KeyFrame, alpha) == 0x48, "KeyFrame::alpha");
    static_assert(__builtin_offsetof(KeyFrame, timestamp) == 0x50, "KeyFrame::timestamp");
    static_assert(__builtin_offsetof(KeyFrame, channelFlags) == 0x58, "KeyFrame::channelFlags");
    static_assert(__builtin_offsetof(KeyFrame, channelFlagsHi) == 0x5c, "KeyFrame::channelFlagsHi");
#endif
}

#endif
