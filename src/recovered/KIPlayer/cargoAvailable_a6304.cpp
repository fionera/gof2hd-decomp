#include "class.h"

extern "C" int KIPlayer_cargoAvailable(KIPlayer *self)
{
    unsigned int *arr = *(unsigned int **)((char *)self + 0x50);
    if (arr != 0) {
        unsigned int len = arr[0];
        unsigned int i = 0;
        while (i < len) {
            int *data = *(int **)((char *)arr + 4);
            if (data[i + 1] > 0) {
                return 1;
            }
            i += 2;
        }
    }
    return 0;
}
