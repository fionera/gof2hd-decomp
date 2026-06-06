#include "class.h"

int Level::getMiningPlant() {
    int index = miningPlantIndex;
    if (index < 0) {
        return 0;
    }
    return ((int *)((Array *)enemies)->data)[index];
}
