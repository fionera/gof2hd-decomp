#include "class.h"

void operator delete[](void *p);

extern "C" int Item_getSinglePrice(void *item);
extern "C" void Globals_addSoundResourceToList(int id);

extern "C" int *g_cws_items;   // DAT_b37da  -> item table (GOT pointer)
extern "C" int *g_cws_sound;    // DAT_b3870  -> sound id holder (deref once)
extern "C" int *g_cws_sound2;   // DAT_b38d8 (deref once)
extern "C" int *g_cws_sound3;   // DAT_b38dc (array base)
extern "C" void Player_calcWeaponSounds_tail(int a, int b);

extern "C" void Player_calcWeaponSounds(Player *self, int count)
{
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (guns->data[0] != 0) {
        unsigned int n = ((Array<Gun *> *)guns->data[0])->length;
        int *order = new int[n];
        for (int i = 0; i < (int)n; i++) {
            order[i] = *(int *)((char *)self->guns->data[0]->data[i] + 0x58);
        }

        bool sorted = true;
        int i = 1;
        int *table = g_cws_items;
        do {
            for (; i < (int)n; i++) {
                int *dataArr = *(int **)((char *)(*(void **)table) + 4);
                int a = Item_getSinglePrice(*(void **)&dataArr[order[i - 1]]);
                dataArr = *(int **)((char *)table + 4);
                int b = Item_getSinglePrice(*(void **)&dataArr[order[i]]);
                if (a < b) {
                    sorted = false;
                    int t = order[i - 1];
                    order[i - 1] = order[i];
                    order[i] = t;
                }
            }
            bool again = !sorted;
            sorted = true;
            i = 1;
            if (!again) break;
        } while (true);

        for (unsigned int x = 0; x < n; x++) {
            for (unsigned int y = 0; y != n; y++) {
                if (x != y && order[x] == order[y]) {
                    order[y] = -1;
                }
            }
        }

        int idx = 0;
        int *sound = g_cws_sound;
        do {
            if ((int)n <= idx) break;
            if (order[idx] >= 0) {
                *(char *)((char *)self->guns->data[0]->data[idx] + 0x89) = 1;
                Globals_addSoundResourceToList(*sound);
                count--;
            }
            idx++;
        } while (count != 0);

        operator delete[](order);
        guns = self->guns;
    }

    if (guns->length > 2) {
        Array<Gun *> *slot2 = guns->data[2];
        if (slot2 != 0 && slot2->length != 0) {
            Gun *g = slot2->data[0];
            if (g != 0) {
                int sid = g_cws_sound3[*(int *)((char *)g + 0x58)];
                *(char *)((char *)g + 0x89) = 1;
                return Player_calcWeaponSounds_tail(*g_cws_sound2, sid);
            }
        }
    }
}
