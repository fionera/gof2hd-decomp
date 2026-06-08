#include "class.h"

// SolarSystem::systemIsInSystemRoutes(int) — self's own system (+0x18) counts; otherwise
// scan the routes array at +0x40 (null -> not present).
extern "C" int SolarSystem_systemIsInSystemRoutes(SolarSystem *self, int sys)
{
    if (self->f_18 != sys) {
        uint32_t *arr = self->f_40;
        if (arr == 0)
            return 0;
        uint32_t n = arr[0];
        for (uint32_t i = 0; i < n; i++) {
            if (((int *)arr[1])[i] == sys)
                return 1;
        }
        return 0;
    }
    return 1;
}
