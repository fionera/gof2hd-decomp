#include "class.h"

namespace AbyssEngine {

void EaseInOut::SetToMinValue() {
    m_t = 1.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
