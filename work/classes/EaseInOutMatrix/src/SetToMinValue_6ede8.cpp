#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::SetToMinValue() {
    this->f_74 = 0.75f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
