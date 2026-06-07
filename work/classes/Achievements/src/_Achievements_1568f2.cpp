#include "class.h"

void operator delete[](void *ptr) noexcept;

// Achievements::~Achievements() — delete[] the two arrays (with null checks),
// zeroing each pointer.
struct Achievements {
    void *medals;     // 0x00
    void *newMedals;  // 0x04
    ~Achievements();
};

Achievements::~Achievements()
{
    if (this->medals != 0)
        operator delete[](this->medals);
    this->medals = 0;
    if (this->newMedals != 0)
        operator delete[](this->newMedals);
    this->newMedals = 0;
}
