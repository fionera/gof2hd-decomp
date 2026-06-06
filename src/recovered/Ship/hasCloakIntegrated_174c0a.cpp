#include "class.h"
extern "C" bool hasCloakIntegrated(Ship *self) { return self->index == 0x2c || self->index == 0x31; }
