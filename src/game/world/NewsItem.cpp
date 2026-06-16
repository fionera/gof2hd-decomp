#include "game/world/NewsItem.h"

NewsItem::NewsItem(int id, bool flag, void *data, int length, int minLevel, int maxLevel)
{
    this->flag = (uint8_t)flag;
    this->id = id;
    this->data = data;
    this->length = length;
    this->minLevel = minLevel;
    this->maxLevel = maxLevel;
    this->used = 0;
}

NewsItem::~NewsItem()
{
    delete[] (uint8_t *)this->data;
    this->data = nullptr;
}

// Deep-copies the byte buffer and the fields into a freshly allocated NewsItem.
NewsItem *NewsItem::clone()
{
    int len = this->length;
    uint8_t *buf = new uint8_t[len];
    for (int i = 0; i < len; i = i + 1) {
        buf[i] = ((uint8_t *)this->data)[i];
    }
    NewsItem *out = new NewsItem(this->id, this->flag != 0, buf, len,
                                this->minLevel, this->maxLevel);
    return out;
}
