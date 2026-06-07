#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_Galaxy_ap_status;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_ap_items;

extern "C" int Galaxy_ap_inAlienOrbit(void *status);
extern "C" int Galaxy_ap_inSupernovaOrbit(void *status);
extern "C" int Galaxy_ap_getCurrentCampaignMission(void *status);
extern "C" void *Galaxy_ap_new(unsigned int size);
extern "C" void Galaxy_ap_delete(void *p);
extern "C" int Galaxy_ap_Station_getSystem(void *station);
extern "C" int Galaxy_ap_SolarSystem_getX(void *sys);
extern "C" int Galaxy_ap_SolarSystem_getY(void *sys);
extern "C" int Galaxy_ap_Item_getMinPriceSystem(void *item);

void *Galaxy::getAsteroidProbabilities(void *station)
{
    int alien = Galaxy_ap_inAlienOrbit(*g_Galaxy_ap_status);
    int supernova = Galaxy_ap_inSupernovaOrbit(*g_Galaxy_ap_status);
    void *systems = alien == 0 ? P(this, 0x4) : 0;
    void *itemTable = *g_Galaxy_ap_items;

    int *probs = (int *)Galaxy_ap_new(0x2c);
    int *ids = (int *)Galaxy_ap_new(0x2c);

    int slot = 0;
    for (int id = 0x9a; id != 0xa4; id = id + 1) {
        ids[slot] = id;
        int prob;
        int next;
        if (alien == 0) {
            int sys = Galaxy_ap_Station_getSystem(station);
            int sysX = Galaxy_ap_SolarSystem_getX(((void **)P(systems, 0x4))[sys]);
            sys = Galaxy_ap_Station_getSystem(station);
            int sysY = Galaxy_ap_SolarSystem_getY(((void **)P(systems, 0x4))[sys]);
            int it = Galaxy_ap_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itX = Galaxy_ap_SolarSystem_getX(((void **)P(systems, 0x4))[it]);
            it = Galaxy_ap_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itY = Galaxy_ap_SolarSystem_getY(((void **)P(systems, 0x4))[it]);
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

    ids[10] = 0xa4;
    probs[10] = alien != 0 ? 100 : 0;

    bool sorted = true;
    int i = 1;
    do {
        for (; i != 0xb; i = i + 1) {
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
    for (int k = 0; k != 0xb; k = k + 1) {
        int v = probs[k];
        if (0 < v)
            probs[k] = v + sub;
        sub = sub - 2;
    }

    int *out = (int *)Galaxy_ap_new(0x58);
    for (unsigned int j = 0; j < 0x16; j = j + 2) {
        out[j] = *(int *)((char *)ids + j * 2);
        out[j + 1] = *(int *)((char *)probs + j * 2);
        if (supernova != 0 && Galaxy_ap_getCurrentCampaignMission(*g_Galaxy_ap_status) > 0x59) {
            out[j] = 0xd9;
        }
    }

    Galaxy_ap_delete(probs);
    Galaxy_ap_delete(ids);
    return out;
}
