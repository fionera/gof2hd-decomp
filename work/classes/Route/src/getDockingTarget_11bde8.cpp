#include "class.h"

// Route::getDockingTarget() -> docking target at the current index, or 0.
extern "C" KIPlayer *Route_getDockingTarget(Route *self)
{
    Array<KIPlayer *> *targets = F<Array<KIPlayer *> *>(self, 0x10);
    int index = self->f_0;
    if (targets != 0 && (int)targets->length > index)
        return targets->data[index];
    return 0;
}
