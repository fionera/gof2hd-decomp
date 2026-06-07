#include "class.h"
struct Vec3 { float x, y, z; };
extern "C" Vec3 AEGeometry_getPosition(void*);
Vec3 PlayerEgo_getPosition(PlayerEgo* self) { return AEGeometry_getPosition(P(self, 8)); }
