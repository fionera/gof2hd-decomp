#include "class.h"

struct PlayerGasCloud {
    void setSparkInSight(int param_1, bool param_2);
};

void PlayerGasCloud::setSparkInSight(int param_1, bool param_2)
{
    void *arr = F<void *>(this, 0x138);
    if (arr == 0)
        return;
    unsigned int len = F<unsigned int>(arr, 0x0);
    if ((unsigned int)param_1 < len) {
        char *base = F<char *>(F<void *>(this, 0x150), 0x4);
        base[param_1] = (char)param_2;
    }
}
