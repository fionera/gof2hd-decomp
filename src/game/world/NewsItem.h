#ifndef GOF2_NEWSITEM_H
#define GOF2_NEWSITEM_H
#include <cstdint>

class NewsItem {
public:
    int id; // +0x00
    uint8_t flag; // +0x04
    void *data; // owned byte buffer (new[]/delete[])   // +0x08
    int length; // buffer length in bytes              // +0x0c
    union {
        int minLevel;
        int field_0x10;
    }; // +0x10 lowest player level this item shows at
    union {
        int maxLevel;
        int field_0x14;
    }; // +0x14 highest player level this item shows at
    union {
        uint8_t used;
        uint8_t field_0x18;
    }; // +0x18 already-displayed flag

    NewsItem(int id, bool flag, bool *data, int length, int field_0x10, int field_0x14);

    ~NewsItem();

    NewsItem *clone();
};
#endif
