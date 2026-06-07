#include "class.h"

struct PlayerGasCloud {
    bool isSparkAlive(int param_1);
};

bool PlayerGasCloud::isSparkAlive(int param_1)
{
    void *arr = F<void *>(this, 0x138);
    if (arr == 0)
        return false;
    if (F<unsigned int>(arr, 0x0) <= (unsigned int)param_1)
        return false;
    int *base = F<int *>(F<void *>(this, 0x14c), 0x4);
    return -1500 < base[param_1];
}
