#include "class.h"

struct Status;
struct Galaxy;
extern "C" uint8_t Station_isDiscovered(Station *self);
extern "C" void Status_visitStation(Status *s);
// tail-called veneer: marks the system visited, given (Galaxy*, systemId)
extern "C" void Galaxy_setSystemVisited(Galaxy *g, int systemId);

extern Status **const gStatusSingleton __attribute__((visibility("hidden")));
extern Galaxy **const gGalaxyVisit __attribute__((visibility("hidden")));

extern "C" void Station_visit(Station *self)
{
    if (Station_isDiscovered(self) != 0)
        return;
    self->f_1c = 1;
    Status_visitStation(*gStatusSingleton);
    Galaxy_setSystemVisited(*gGalaxyVisit, self->f_c);
}
