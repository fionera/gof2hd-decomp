#include "class.h"

namespace AbyssEngine {

EaseInOut::EaseInOut() {
    m_min = 0.0f;
    m_range = 2.0f * PI;
    m_t = 1.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
