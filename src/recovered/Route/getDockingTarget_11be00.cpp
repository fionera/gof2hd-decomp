#include "class.h"

// Route::getDockingTarget(int) -> docking target at the given index, or 0.
extern "C" KIPlayer *Route_getDockingTarget_i(Route *self, int index)
{
    Array<KIPlayer *> *targets = F<Array<KIPlayer *> *>(self, 0x10);
    if (targets != 0 && index < (int)targets->length)
        return targets->data[index];
    return 0;
}
