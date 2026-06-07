#include "class.h"

struct ParticleSystemMesh {
    void setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c, float d,
                     bool trail, float e, float f, const Vector &dir);
    void setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c, float d,
                     bool trail, float e, float f, const Vector &dir, bool finished);
};

void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c,
                                     float d, bool trail, float e, float f, const Vector &dir)
{
    return setParticle(pos, scale, color, a, b, c, d, trail, e, f, dir, false);
}
