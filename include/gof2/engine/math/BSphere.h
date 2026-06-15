#ifndef GOF2_BSPHERE_H
#define GOF2_BSPHERE_H
#include <cstdint>

// AbyssEngine::AEMath::BSphere -- bounding sphere (center + radius, plus a looser
// maxRadius bound used while growing the sphere over a transform hierarchy).

namespace AbyssEngine {

class Transform;

namespace AEMath {

class BSphere {
public:
    float cx, cy, cz;   // center
    float radius;
    float maxRadius;    // second, looser radius bound

    BSphere &operator=(const BSphere &other);

    // Grow *this so it also encloses the world-space bounds of `t`.
    void Merge(const Transform &t);
    // Grow *this so it also encloses `other`.
    void Merge(const BSphere &other);
};

} // namespace AEMath
} // namespace AbyssEngine
#endif // GOF2_BSPHERE_H
