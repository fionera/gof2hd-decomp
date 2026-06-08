#include "class.h"

// AEGeometry::isVisible()  ->  ldrb.w r0,[r0,#0x48] ; bx lr
struct AEGeometry { uint8_t isVisible(); };
uint8_t AEGeometry::isVisible() { return this->f_48; }
