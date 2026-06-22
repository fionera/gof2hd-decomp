#ifndef GOF2_BSPHERE_H
#define GOF2_BSPHERE_H
#include <cstdint>

namespace AbyssEngine {
    namespace AEMath {
        class BSphere {
        public:
            float cx, cy, cz;
            float radius;
            float maxRadius;

            BSphere &operator=(const BSphere &other);

            void Merge(const Transform &t);

            void Merge(const BSphere &other);
        };
    }
}
#endif
