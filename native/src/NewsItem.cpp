#include "NewsItem.h"




// ---- clone_e09fa.cpp ----
// NewsItem::clone()
//   Deep-copies the byte buffer (length = this->0x0c, sign-corrected) and allocates a new
//   0x1c-byte NewsItem with the copied fields.
struct NewsItem { void *clone(); };
void *NewsItem::clone()
{
    int len = i32(this, 0x0c);
    // operator new[]( len | (len >> 31) ) -- the (n | n>>31) idiom from the target.
    char *buf = (char *)operator new[]((uint32_t)(len | (len >> 31)));
    for (int i = 0; i < len; i = i + 1) {
        buf[i] = ((char *)pp(this, 0x08))[i];
    }
    void *out = operator new(0x1c);
    int f10 = i32(this, 0x10);
    int f14 = i32(this, 0x14);
    int id = i32(this, 0x00);
    u8(out, 0x04) = u8(this, 0x04);
    i32(out, 0x00) = id;
    pp(out, 0x08) = buf;
    i32(out, 0x0c) = len;
    i32(out, 0x10) = f10;
    i32(out, 0x14) = f14;
    u8(out, 0x18) = 0;
    return out;
}

// ---- NewsItem_e09c8.cpp ----
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

// ---- _NewsItem_e09e6.cpp ----
// NewsItem::~NewsItem()
//   delete[] this->data; this->data = 0;
struct NewsItem { ~NewsItem(); };
NewsItem::~NewsItem()
{
    operator delete[](pp(this, 0x08));
    pp(this, 0x08) = 0;
}
