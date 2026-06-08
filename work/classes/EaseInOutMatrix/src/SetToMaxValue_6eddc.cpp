#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetToMaxValue() {
    this->f_74 = 1.25f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
