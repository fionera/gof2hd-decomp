#include "class.h"

typedef void (*VectorAssignFn)(Vector *self, const Vector *other);

__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_projectionScratch;
__attribute__((visibility("hidden"))) extern VectorAssignFn PlayerAsteroid_vectorAssign;

extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);
extern "C" Vector *Vector_sub_assign(Vector *self, const Vector *other);
extern "C" Vector VectorNormalize(const Vector *vector);

Vector PlayerAsteroid::getProjectionVector(const Vector &value)
{
    Vector result = value;
    Vector position = AEGeometry_getPosition(F<AEGeometry *>(this, 0x8));
    Vector *scratch = PlayerAsteroid_projectionScratch;
    VectorAssignFn assign = PlayerAsteroid_vectorAssign;
    assign(scratch, &position);
    Vector_sub_assign(scratch, &result);
    assign(&result, scratch);
    Vector normalized = VectorNormalize(&result);
    assign(&result, &normalized);
    return result;
}
