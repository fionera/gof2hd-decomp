#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_Galaxy_pp_status;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_pp_items;

extern "C" int Galaxy_pp_inAlienOrbit(void *status);
extern "C" void *Galaxy_pp_new(unsigned int size);
extern "C" void Galaxy_pp_delete(void *p);
extern "C" int Galaxy_pp_Station_getSystem(void *station);
extern "C" int Galaxy_pp_SolarSystem_getX(void *sys);
extern "C" int Galaxy_pp_SolarSystem_getY(void *sys);
extern "C" int Galaxy_pp_Item_getMinPriceSystem(void *item);

void *Galaxy::getPlasmaProbabilities(void *station)
{
    int alien = Galaxy_pp_inAlienOrbit(*g_Galaxy_pp_status);
    void *systems = alien == 0 ? this->systems : 0;
    void *itemTable = *g_Galaxy_pp_items;

    int *probs = (int *)Galaxy_pp_new(0x10);
    int *ids = (int *)Galaxy_pp_new(0x10);

    int slot = 0;
    for (int id = 0xc9; id != 0xcd; id = id + 1) {
        ids[slot] = id;
        int prob;
        int next;
        if (alien == 0) {
            int sys = Galaxy_pp_Station_getSystem(station);
            int sysX = Galaxy_pp_SolarSystem_getX(((void **)systems->systems)[sys]);
            sys = Galaxy_pp_Station_getSystem(station);
            int sysY = Galaxy_pp_SolarSystem_getY(((void **)systems->systems)[sys]);
            int it = Galaxy_pp_Item_getMinPriceSystem(((void **)itemTable->systems)[id]);
            int itX = Galaxy_pp_SolarSystem_getX(((void **)systems->systems)[it]);
            it = Galaxy_pp_Item_getMinPriceSystem(((void **)itemTable->systems)[id]);
            int itY = Galaxy_pp_SolarSystem_getY(((void **)systems->systems)[it]);
            prob = invDistancePercent(sysX, sysY, itX, itY);
            next = slot + 1;
            if (prob < 0x32)
                prob = 0;
        } else {
            prob = 0;
            next = slot;
        }
        probs[slot] = prob;
        slot = next;
    }

    bool sorted = true;
    int i = 1;
    do {
        for (; i != 4; i = i + 1) {
            int prev = i - 1;
            int a = probs[i];
            int b = probs[prev];
            if (b < a) {
                probs[prev] = a;
                int idPrev = ids[prev];
                int idCur = ids[i];
                probs[i] = b;
                ids[prev] = idCur;
                ids[i] = idPrev;
                sorted = false;
            }
        }
        bool again = !sorted;
        sorted = true;
        i = 1;
        if (!again)
            break;
    } while (true);

    int sub = 0;
    for (int k = 0; k != 4; k = k + 1) {
        int v = probs[k];
        if (0 < v)
            probs[k] = v + sub;
        sub = sub - 2;
    }

    int *out = (int *)Galaxy_pp_new(0x20);
    for (unsigned int j = 0; j <= 7; j = j + 2) {
        out[j] = *(int *)((char *)ids + j * 2);
        out[j + 1] = *(int *)((char *)probs + j * 2);
    }

    Galaxy_pp_delete(probs);
    Galaxy_pp_delete(ids);
    return out;
}
