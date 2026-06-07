#include "class.h"

struct Sparks {
    bool isRocket();
};

bool Sparks::isRocket()
{
    return F<int>(this, 0x14) == 1;
}
