#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetToMinValue() {
    f32(this, 0x74) = 0.75f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
