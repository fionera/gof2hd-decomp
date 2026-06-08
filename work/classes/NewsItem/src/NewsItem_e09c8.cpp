#include "class.h"

// NewsItem::NewsItem(int, bool, bool*, int, int, int)
struct NewsItem { NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6); };
NewsItem::NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6)
{
    u8(this, 0x04) = (uint8_t)p2;
    i32(this, 0x00) = p1;
    pp(this, 0x08) = p3;
    i32(this, 0x0c) = p4;
    i32(this, 0x10) = p5;
    i32(this, 0x14) = p6;
    u8(this, 0x18) = 0;
}
