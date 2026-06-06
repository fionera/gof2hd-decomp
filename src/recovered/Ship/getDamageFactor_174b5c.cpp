#include "class.h"
extern "C" int getDamageFactor(Ship *self) { return *(int *)&self->damageFactor; }
