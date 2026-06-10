#ifndef GOF2_EASEINOUT_H
#define GOF2_EASEINOUT_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
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
#endif
