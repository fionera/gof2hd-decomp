#include "class.h"

extern "C" void Globals_addSoundResource_tail(int val, Array<int> *a);

// r0=container, r1=value. Linear search; on no-match (index reaches length) tail-call to add.
extern "C" void Globals_addSoundResourceToList(void *self, int val)
{
    Array<int> *a = F<Array<int> *>(self, 4);
    if (a != 0) {
        unsigned i = 0;
        for (;;) {
            if (i >= a->length) {
                return Globals_addSoundResource_tail(val, a);
            }
            int e = a->data[i];
            i = i + 1;
            if (e == val) {
                break;
            }
        }
    }
}
