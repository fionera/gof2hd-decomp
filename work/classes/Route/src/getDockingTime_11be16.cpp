#include "class.h"

extern "C" int Route_getDockingTimeAt(Route *self, int index);

// Route::getDockingTime() -> docking time at the current index.
extern "C" int Route_getDockingTime(Route *self)
{
    return Route_getDockingTimeAt(self, F<int32_t>(self, 0x0));
}
