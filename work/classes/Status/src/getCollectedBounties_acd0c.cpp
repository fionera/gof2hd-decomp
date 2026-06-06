#include "class.h"
int Status::getCollectedBounties(int index) {
    if (index < 4) return collectedBounties[index];
    return 0;
}
