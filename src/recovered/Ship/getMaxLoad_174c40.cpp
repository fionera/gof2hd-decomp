#include "class.h"
extern "C" int getMaxLoad(Ship *self) { return self->cargoPlus + self->baseLoad; }
