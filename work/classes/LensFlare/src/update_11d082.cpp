#include "class.h"

// LensFlare::update()
//   this->intensity(0x00) = 0;   (movs r1,#0 ; str r1,[r0,#0x0] ; bx lr)
struct LensFlare {
    // @portable-fields
    int f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
 void update(); };
void LensFlare::update()
{
    this->f_0 = 0;
}
