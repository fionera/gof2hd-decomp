#include "class.h"
struct Vec3 { float x, y, z; };
extern "C" Vec3 AEGeometry_getDirection(void*);
Vec3 PlayerEgo_GetDirVector(PlayerEgo* self) { return AEGeometry_getDirection(P(self, 8)); }
