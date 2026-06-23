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
        uint32_t timestampLo;
        uint32_t timestampHi;
        uint32_t field_0x58;
        uint32_t field_0x5c;

        KeyFrame();
    };
}

#endif
