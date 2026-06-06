#include "class.h"
extern "C" unsigned int hasModInstalled(Ship *self, int mod) {
    unsigned int *m = (unsigned int *)self->mods;
    if (m != 0) {
        unsigned int i = 0;
        while (i < *m) {
            int idx = i * 4;
            i = i + 1;
            if (*(int *)(m[1] + idx) == mod) {
                return 1;
            }
        }
    }
    return 0;
}
