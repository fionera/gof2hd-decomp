#include "class.h"

namespace AbyssEngine {

void EaseInOutMatrix::RunOut(float dt) {
    float target = 1.0f;
    float t = f32(this, 0x74);
    if (t > target) {
        t = t + (dt * -0.5f) / f32(this, 0xf0);
        f32(this, 0x74) = t;
        if (t < target) {
            f32(this, 0x74) = 1.0f;
        }
    } else if (t < target) {
        t = t + (dt * 0.5f) / f32(this, 0xf0);
        f32(this, 0x74) = t;
        if (t > target) {
            f32(this, 0x74) = 1.0f;
        }
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine
