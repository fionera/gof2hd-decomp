#include "class.h"

extern "C" __attribute__((visibility("hidden"))) const volatile int
    kaamoSpecialItems[9];

bool Generator::isKaamoSpecialItem(int item) {
    for (uint32_t i = 0;;) {
        if (i > 8) {
            return false;
        }
        int current = kaamoSpecialItems[i];
        ++i;
        if (current == item) {
            return true;
        }
    }
}
