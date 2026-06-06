#include "class.h"

// Adjusts the player rating by `delta`, clamping the result to [-10, 10].
void Status::changeRating(int delta) {
    int updated = delta + rating;
    rating = updated;
    int clamped;
    if (updated < 0xb) {
        if (-0xb < updated) {
            return;
        }
        clamped = -10;
    } else {
        clamped = 10;
    }
    rating = clamped;
}
