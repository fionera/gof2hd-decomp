#ifndef GOF2_KEYFRAME_H
#define GOF2_KEYFRAME_H

#include "engine/core/Array.h"
#include "engine/math/Vector.h"

namespace AbyssEngine {
    class KeyFrame {
    public:
        AEMath::Vector translation; // +0x00  world translation
        AEMath::Vector scale; // +0x0c  world scale (default 1,1,1)
        AEMath::Vector rotation; // +0x18  world rotation (euler, fed to Quaternion)
        AEMath::Vector localTranslation; // +0x24  local translation
        AEMath::Vector localScale; // +0x30  local scale (default 1,1,1)
        AEMath::Vector localRotation; // +0x3c  local rotation (euler, fed to Quaternion)
        float alpha; // +0x48  blend weight / alpha (default 1.0)
        uint32_t field_0x4c; // +0x4c  left uninitialized by the ctor
        uint32_t timestampLo; // +0x50  key time (low word)
        uint32_t timestampHi; // +0x54  key time (high word)
        uint32_t field_0x58; // +0x58
        uint32_t field_0x5c; // +0x5c

        KeyFrame();
    };
} // namespace AbyssEngine

#endif
