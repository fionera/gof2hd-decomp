#ifndef GOF2_EASEINOUT_H
#define GOF2_EASEINOUT_H

namespace AbyssEngine {

namespace AEMath {
float Sinf(float value);
}

// PI as a 32-bit float constant. Double-context uses promote it to double,
// reproducing the engine's literal-pool double values (N * (double)PI).
static const float PI = 3.1415927f;

class EaseInOut {
public:
    float m_min;      // base/start output value
    float m_range;    // span (max - min)
    float m_t;        // angle parameter (radians)
    float m_current;  // current output value

    EaseInOut();
    EaseInOut(float minValue, float maxValue);

    void SetRange(float minValue, float maxValue);
    void SetToMinValue();
    void SetToMaxValue();
    float GetMaxValue();

    void Increase(float dt);
    void Decrease(float dt);
    void RunOut(float dt);
    void UpdateCurrentValue();

    // Per-frame stepper: advance the ease toward its maximum and recompute output.
    void Update(float dt);
    // Current animated output value.
    float GetValue();
    float GetCurrentValue();
    // Base (start) output value.
    float GetMinValue();
};

} // namespace AbyssEngine

#endif
