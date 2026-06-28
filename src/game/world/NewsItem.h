#ifndef GOF2_NEWSITEM_H
#define GOF2_NEWSITEM_H
#include <cstdint>

class NewsItem {
public:
    int id;
    uint8_t flag;
    bool *data;
    int length;
    int minLevel;
    int maxLevel;
    uint8_t used;

    NewsItem(int id, bool flag, bool *data, int length, int field_0x10, int field_0x14);

    ~NewsItem();

    NewsItem *clone();
};
#endif
