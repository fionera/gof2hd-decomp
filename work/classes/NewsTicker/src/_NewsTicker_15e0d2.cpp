#include "class.h"

NewsTicker::~NewsTicker()
{
    ((String *)((char *)this + 0x14))->~String();
}
