#include "class.h"

typedef Vector (*VirtualVectorFn)(PlayerAsteroid *self);

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_random;

extern "C" Vector Vector_sub(const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *vector);
extern "C" Vector VectorNormalize(const Vector *vector);
extern "C" Vector Vector_scale(const Vector *vector, float scale);
extern "C" Vector *Vector_assign(Vector *self, const Vector *other);
extern "C" int AERandom_nextInt(void *random, int max);

void PlayerAsteroid::initPush(const Vector &target, int duration)
{
    VirtualVectorFn getVector = *(VirtualVectorFn *)((char *)*(void **)this + 0x28);
    Vector current = getVector(this);
    Vector delta = Vector_sub(&target, &current);
    float ratio = VectorLength(&delta) / (float)duration;
    float clamped = 1.0f;
    if (ratio < 1.0f) {
        clamped = ratio;
    }
    int pushFrames = (int)((1.0f - clamped) * 1000.0f);
    this->f_104 = pushFrames;
    this->f_108 = pushFrames;

    Vector here = getVector(this);
    Vector directionSource = Vector_sub(&here, &target);
    Vector direction = VectorNormalize(&directionSource);
    Vector_assign((Vector *)((char *)this + 0x10c), &direction);

    void *random = PlayerAsteroid_random;
    Vector randomVector = {
        (float)(AERandom_nextInt(random, 200) - 100),
        (float)(AERandom_nextInt(random, 200) - 100),
        (float)(AERandom_nextInt(random, 200) - 100),
    };
    Vector randomDirection = VectorNormalize(&randomVector);
    Vector randomScaled = Vector_scale(&randomDirection, 0.01f);
    Vector_assign((Vector *)((char *)this + 0x118), &randomScaled);
}
