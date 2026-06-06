#include "class.h"
void Status::incCollectedBounties(int index) {
    if (index < 4) {
        collectedBounties[index] = collectedBounties[index] + 1;
    }
}
