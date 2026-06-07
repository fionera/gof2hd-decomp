#include "class.h"

namespace AbyssEngine {

void EaseInOut::SetRange(float minValue, float maxValue) {
    m_t = 1.5f * PI;
    m_min = minValue;
    m_range = maxValue - minValue;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
