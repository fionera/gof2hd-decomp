#include "class.h"

namespace AbyssEngine {

void EaseInOut::SetToMaxValue() {
    m_t = 2.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
