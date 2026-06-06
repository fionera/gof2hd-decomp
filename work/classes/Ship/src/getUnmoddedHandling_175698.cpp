#include "class.h"
extern "C" int getUnmoddedHandling(Ship *self) { return *(int *)&self->handling; }
