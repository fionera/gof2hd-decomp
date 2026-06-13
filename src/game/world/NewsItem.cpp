#include "gof2/game/world/NewsItem.h"

// NewsItem::clone()
//   Deep-copies the byte buffer (length = this->length, sign-corrected) and allocates a new
//   0x1c-byte NewsItem with the copied fields.
void *NewsItem::clone()
{
    int len = this->length;
    // operator new[]( len | (len >> 31) ) -- the (n | n>>31) idiom from the target.
    char *buf = (char *)operator new[]((uint32_t)(len | (len >> 31)));
    for (int i = 0; i < len; i = i + 1) {
        buf[i] = ((char *)this->data)[i];
    }
    NewsItem *out = (NewsItem *)operator new(0x1c);
    int f10 = this->field_0x10;
    int f14 = this->field_0x14;
    int id = this->id;
    out->flag = this->flag;
    out->id = id;
    out->data = buf;
    out->length = len;
    out->field_0x10 = f10;
    out->field_0x14 = f14;
    out->field_0x18 = 0;
    return out;
}

// NewsItem::NewsItem(int, bool, bool*, int, int, int)
NewsItem::NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6)
{
    this->flag = (byte)p2;
    this->id = p1;
    this->data = p3;
    this->length = p4;
    this->field_0x10 = p5;
    this->field_0x14 = p6;
    this->field_0x18 = 0;
}

// NewsItem::~NewsItem()
//   delete[] this->data; this->data = 0;
NewsItem::~NewsItem()
{
    operator delete[](this->data);
    this->data = 0;
}
