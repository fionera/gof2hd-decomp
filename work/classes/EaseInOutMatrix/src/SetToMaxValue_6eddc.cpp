#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetToMaxValue() {
    f32(this, 0x74) = 1.25f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
