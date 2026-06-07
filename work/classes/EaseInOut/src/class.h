#ifndef GOF2_DECOMP_EASEINOUT_CLASS_H
#define GOF2_DECOMP_EASEINOUT_CLASS_H

namespace AbyssEngine {

namespace AEMath {
float Sinf(float value);
}

// PI as a 32-bit float constant; double-context uses promote it to double,
// reproducing the target's literal-pool double values (N * (double)PI).
static const float PI = 3.1415927f;

struct EaseInOut {
    float m_min;      // +0x0  base/start output value
    float m_range;    // +0x4  span (max - min)
    float m_t;        // +0x8  angle parameter (radians)
    float m_current;  // +0xc  current output value

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
};

} // namespace AbyssEngine

#endif
