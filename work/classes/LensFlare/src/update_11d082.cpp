#include "class.h"

// LensFlare::update()
//   this->intensity(0x00) = 0;   (movs r1,#0 ; str r1,[r0,#0x0] ; bx lr)
struct LensFlare { void update(); };
void LensFlare::update()
{
    i32(this, 0x00) = 0;
}
