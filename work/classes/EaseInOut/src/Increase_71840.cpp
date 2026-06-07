#include "class.h"

namespace AbyssEngine {

void EaseInOut::Increase(float dt) {
    m_t = (float)((double)m_t + (double)dt * (1.0 / 65536.0) * (2.0 * PI));
    if ((double)m_t > 2.5 * PI) {
        m_t = 2.5f * PI;
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine
