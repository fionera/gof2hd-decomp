#include "class.h"
extern "C" bool spaceAvailable(Ship *self, int n) { return n + self->currentLoad <= self->baseLoad + self->cargoPlus; }
