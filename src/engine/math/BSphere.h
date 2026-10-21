#ifndef GOF2_BSPHERE_H
#define GOF2_BSPHERE_H
#include "engine/math/Vector.h"

namespace AbyssEngine {
    class Transform;

    namespace AEMath {
        struct BSphere {
            Vector center;
            float radius;
            float radius2;

            BSphere() : center(), radius(0.0f), radius2(1.0f) {}

            BSphere(const Vector &center, float radius, float radius2)
                : center(center), radius(radius), radius2(radius2) {}

            BSphere &operator=(const BSphere &other);

            void Merge(const BSphere &other);

            void Merge(const Transform &t);
        };
    }
}

#endif
