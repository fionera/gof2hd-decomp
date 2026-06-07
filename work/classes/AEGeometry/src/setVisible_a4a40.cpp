#include "class.h"

// AEGeometry::setVisible(bool)  ->  strb.w r1,[r0,#0x49] ; strb.w r1,[r0,#0x48] ; bx lr
struct AEGeometry { void setVisible(bool v); };
void AEGeometry::setVisible(bool v)
{
    u8(this, 0x48) = v;
    u8(this, 0x49) = v;
}
