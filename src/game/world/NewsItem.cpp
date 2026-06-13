#include "gof2/NewsItem.h"


// ---- clone_e09fa.cpp ----
// NewsItem::clone()
//   Deep-copies the byte buffer (length = this->field_0x0c, sign-corrected) and allocates a new
//   0x1c-byte NewsItem with the copied fields.
void *NewsItem::clone()
{
    int len = this->field_0x0c;
    // operator new[]( len | (len >> 31) ) -- the (n | n>>31) idiom from the target.
    char *buf = (char *)operator new[]((uint32_t)(len | (len >> 31)));
    for (int i = 0; i < len; i = i + 1) {
        buf[i] = ((char *)this->field_0x08)[i];
    }
    NewsItem *out = (NewsItem *)operator new(0x1c);
    int f10 = this->field_0x10;
    int f14 = this->field_0x14;
    int id = this->field_0x00;
    out->field_0x04 = this->field_0x04;
    out->field_0x00 = id;
    out->field_0x08 = buf;
    out->field_0x0c = len;
    out->field_0x10 = f10;
    out->field_0x14 = f14;
    out->field_0x18 = 0;
    return out;
}

// ---- NewsItem_e09c8.cpp ----
// NewsItem::NewsItem(int, bool, bool*, int, int, int)
NewsItem::NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6)
{
    this->field_0x04 = (byte)p2;
    this->field_0x00 = p1;
    this->field_0x08 = p3;
    this->field_0x0c = p4;
    this->field_0x10 = p5;
    this->field_0x14 = p6;
    this->field_0x18 = 0;
}

// ---- _NewsItem_e09e6.cpp ----
// NewsItem::~NewsItem()
//   delete[] this->data; this->data = 0;
NewsItem::~NewsItem()
{
    operator delete[](this->field_0x08);
    this->field_0x08 = 0;
}
