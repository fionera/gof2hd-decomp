#include "class.h"
extern "C" int getFreeSpace(Ship *self) { return (self->cargoPlus + self->baseLoad) - self->currentLoad; }
