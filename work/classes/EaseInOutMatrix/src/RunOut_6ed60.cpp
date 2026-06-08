#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::RunOut(float dt) {
    float target = 1.0f;
    float t = this->f_74;
    if (t > target) {
        t = t + (dt * -0.5f) / this->f_f0;
        this->f_74 = t;
        if (t < target) {
            this->f_74 = 1.0f;
        }
    } else if (t < target) {
        t = t + (dt * 0.5f) / this->f_f0;
        this->f_74 = t;
        if (t > target) {
            this->f_74 = 1.0f;
        }
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine
