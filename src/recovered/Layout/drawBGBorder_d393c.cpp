#include "class.h"

// 9-arg impl. Forwarded: (p5 as self, p2, p3, p4, p5, p6, instack, 0, 0).
extern "C" void Layout_drawBGBorderImpl(int self, unsigned p2, int p3, int p4,
                                        int p5, int p6, int s8, int z1, int z2); // 0x74e30

// Layout::drawBGBorder(uint p1, uint p2, int p3, int p4, int p5, int p6) + stack arg s8
extern "C" void Layout_drawBGBorder6(unsigned p1, unsigned p2, int p3, int p4,
                                     int p5, int p6, int s8) {
    Layout_drawBGBorderImpl(p5, p2, p3, p4, p5, p6, s8, 0, 0);
}
