#include "class.h"

// NewsItem::~NewsItem()
//   delete[] this->data; this->data = 0;
struct NewsItem { ~NewsItem(); };
NewsItem::~NewsItem()
{
    operator delete[](pp(this, 0x08));
    pp(this, 0x08) = 0;
}
