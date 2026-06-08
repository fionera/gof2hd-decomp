#include "class.h"

struct LODManager {
    void update(int dt);
    void forceUpdate(int a, bool b);
};

void LODManager::update(int dt)
{
    int sum = F<int>(this, 0x10) + dt;
    F<int>(this, 0x10) = sum;
    if (sum > 1000) {
        F<int>(this, 0x10) = 0;
        return forceUpdate(0, false);
    }
}
