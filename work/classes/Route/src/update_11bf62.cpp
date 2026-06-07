#include "class.h"

extern "C" void Route_update_xyz(Route *self, float x, float y, float z);

// Route::update(Vector const&) -> tail-call update(v.x, v.y, v.z).
extern "C" void Route_update(Route *self, const Vector &v)
{
    return Route_update_xyz(self, v.x, v.y, v.z);
}
