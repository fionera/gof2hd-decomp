#include "class.h"

namespace AbyssEngine {

void EaseInOut::UpdateCurrentValue() {
    if ((double)m_t == 2.5 * PI) {
        m_current = m_min + m_range;
    } else {
        double s = (double)AEMath::Sinf(m_t) * 0.5 + 0.5;
        m_current = (float)((double)m_min + s * (double)m_range);
    }
}

} // namespace AbyssEngine
