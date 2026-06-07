#include "class.h"
struct Vec3 { float x, y, z; };
extern "C" Vec3 AEGeometry_getUpVector(void*);
Vec3 PlayerEgo_GetUpVector(PlayerEgo* self) { return AEGeometry_getUpVector(P(self, 8)); }
