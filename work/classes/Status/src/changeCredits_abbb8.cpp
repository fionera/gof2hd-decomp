#include "class.h"

// Credit deltas whose magnitude exceeds 1e9 are ignored; the balance is clamped at >= 0.
__attribute__((minsize)) void Status::changeCredits(int delta) {
    int magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude <= 1000000000) {
        unsigned credited = (unsigned)(delta + credits);
        credits = credited & ~((int)credited >> 0x1f);
    }
}
