#include "class.h"
extern "C" int getFireRateFactor(Ship *self) { return *(int *)&self->fireRateFactor; }
