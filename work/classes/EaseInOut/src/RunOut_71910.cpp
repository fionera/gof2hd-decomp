#include "class.h"

namespace AbyssEngine {

void EaseInOut::RunOut(float dt) {
    float target = 2.0f * PI;
    float t = m_t;
    if (t > target) {
        t = (float)((double)t + (double)dt * (-1.0 / 65536.0) * (2.0 * PI));
        m_t = t;
        if (t < target) {
            m_t = target;
        }
    } else if (t < target) {
        t = (float)((double)t + (double)dt * (1.0 / 65536.0) * (2.0 * PI));
        m_t = t;
        if (t > target) {
            m_t = target;
        }
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine
