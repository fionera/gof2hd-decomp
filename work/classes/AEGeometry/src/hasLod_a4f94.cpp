#include "class.h"

// AEGeometry::hasLod()  ->  ldr; cmp; it ne; movne #1; bx lr
struct AEGeometry { bool hasLod(); };
bool AEGeometry::hasLod() { return this->f_54 != 0; }
