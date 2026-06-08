#include "class.h"

extern "C" void *operator_new__(unsigned int sz);   // operator new[]
extern "C" void operator_delete__(void *p);          // operator delete[]
extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayKIPlayer_ctor(void *a);
extern "C" void ArraySetLengthKIPlayer(uint32_t n, void *a);
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count);
extern "C" Route *Route_ctor1(Route *self, int *coords, int count);

// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
extern "C" Route *Route_clone(Route *self)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    int n = (int)wps->length;
    int *coords = (int *)operator_new__(n * 3 * 4);
    int *p = coords;
    for (int i = 0; i != n; i++) {
        Waypoint *wp = wps->data[i];
        p[0] = *(int32_t *)((char *)wp + 0x124);
        p[1] = *(int32_t *)((char *)wp + 0x128);
        p[2] = *(int32_t *)((char *)wp + 0x12c);
        p += 3;
    }
    Array<int> *tgt = F<Array<int> *>(self, 0x10);
    if (tgt != 0) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->length; k++)
            if (tgt->data[k] != 0)
                any = true;
        if (any) {
            Array<int> *times = F<Array<int> *>(self, 0x14);
            int *timesCopy = (int *)operator_new__(times->length * 4);
            for (uint32_t k = 0; k < times->length; k++)
                timesCopy[k] = times->data[k];
            void *targets = operator_new(0xc);
            ArrayKIPlayer_ctor(targets);
            ArraySetLengthKIPlayer(F<Array<void *> *>(self, 0x10)->length, targets);
            for (uint32_t k = 0; k < F<Array<void *> *>(self, 0x10)->length; k++)
                ((void **)*(void **)((char *)targets + 4))[k] = F<Array<void *> *>(self, 0x10)->data[k];
            Route *r = (Route *)operator_new(0x18);
            Route_ctor2(r, coords, targets, timesCopy, (int)F<Array<Waypoint *> *>(self, 0xc)->length * 3);
            r->f_4 = self->f_4;
            operator_delete__(timesCopy);
            return r;
        }
    }
    Route *r = (Route *)operator_new(0x18);
    Route_ctor1(r, coords, wps->length * 3);
    r->f_4 = self->f_4;
    return r;
}
