#include "engine/math/AEMath.h"

#include <cmath>
#include <cstring>

namespace AbyssEngine {
    namespace AEMath {
        float Sqrtf(float value) { return ::sqrtf(value); }
        float Sinf(float value) { return ::sinf(value); }
        float Cosf(float value) { return ::cosf(value); }
        float ACosf(float value) { return ::acosf(value); }
        float ATanf(float value) { return ::atanf(value); }
        float Pow(float lhs, float rhs) { return ::powf(lhs, rhs); }

        float Absf(float value) {
            if (value >= 0.0f)
                return value;
            return -value;
        }

        float Max(float lhs, float rhs) {
            return lhs > rhs ? lhs : rhs;
        }

        float Min(float lhs, float rhs) {
            return lhs < rhs ? lhs : rhs;
        }

        float InvSqrt(float value) {
            float f = value;
            int i;
            std::memcpy(&i, &f, sizeof(i));
            i = 0x5f3759df - (i >> 1);
            std::memcpy(&f, &i, sizeof(f));
            f = f * (1.5f - value * 0.5f * f * f);
            return f;
        }

    }
}
