#ifndef GOF2_NEWSITEM_H
#define GOF2_NEWSITEM_H
#include <cstdint>

// A single ticker entry: an id, a flag, and an owned byte buffer with its length.
class NewsItem {
public:
    int      id;
    uint8_t  flag;
    void    *data;       // owned byte buffer (new[]/delete[])
    int      length;     // buffer length in bytes
    int      field_0x10;
    int      field_0x14;
    uint8_t  field_0x18;

    NewsItem(int id, bool flag, void *data, int length, int field_0x10, int field_0x14);
    ~NewsItem();

    NewsItem *clone();
};
#endif
