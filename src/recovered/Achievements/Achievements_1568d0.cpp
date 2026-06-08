#include "class.h"

void *operator new[](__SIZE_TYPE__ size);

// Achievements::Achievements() — allocates two 45-int (0xb4) arrays.
struct Achievements {
    void *medals;     // 0x00
    void *newMedals;  // 0x04
    Achievements();
};

Achievements::Achievements()
{
    this->medals = operator new[](0xb4);
    this->newMedals = operator new[](0xb4);
    *(uint8_t *)((char *)this + 0x22) = 0;
    *(uint16_t *)((char *)this + 0x20) = 0;
}
