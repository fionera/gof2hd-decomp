#include "engine/core/KeyFrame.h"

namespace AbyssEngine {

// Seeds both scale channels to (1,1,1) and the blend weight to 1.0, and
// zeroes the remaining transform channels, timestamp and trailing words.
KeyFrame::KeyFrame()
{
    const AEMath::Vector zero = {0.0f, 0.0f, 0.0f};
    const AEMath::Vector one = {1.0f, 1.0f, 1.0f};

    translation = zero;
    scale = one;
    rotation = zero;
    localTranslation = zero;
    localScale = one;
    localRotation = zero;
    alpha = 1.0f;

    _pad_4c = 0;
    timestamp = 0;
    field_0x58 = 0;
    field_0x5c = 0;
}

} // namespace AbyssEngine
