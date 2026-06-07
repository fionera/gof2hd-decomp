#include "class.h"

struct Status {
    int getShip();
};

extern "C" int Ship_getFirstEquipmentOfSort(int ship, int sort);
extern "C" int Level_opnew(unsigned int size);
extern "C" void ArrayCtor(int arr);
extern "C" void ArraySetLength(int len, int arr);
extern "C" unsigned int uidiv(unsigned int a, unsigned int b);
extern "C" int Level_createStaticObject_call(Level *self, int wp, int type, int flag);
extern "C" void Player_setRadius(int player, int radius);
extern "C" void Player_setAlwaysFriend(int player, int flag);
extern "C" void Player_setMaxHitpoints(int player, int hp);
extern "C" void KIPlayer_setActive(int k, int flag);
extern "C" void ArrayAdd(int item, int arr);

__attribute__((visibility("hidden"))) extern Status **g_sentryStatus;

void Level::createSentryGuns() {
    Status **slot = g_sentryStatus;
    int ship = (*slot)->getShip();
    if (Ship_getFirstEquipmentOfSort(ship, 0x27) != 0) {
        int guns = Level_opnew(0xc);
        ArrayCtor(guns);
        field_b0 = guns;
        ArraySetLength(9, guns);
        if (enemies == 0) {
            int e = Level_opnew(0xc);
            ArrayCtor(e);
            enemies = e;
        }
        int color = 0x9923e035;
        for (unsigned int i = 0; i < *(unsigned int *)field_b0; i = i + 1) {
            int obj = Level_createStaticObject_call(this, 0, uidiv(i, 3) + 0x49c0, 1);
            ((int *)(*(int *)(field_b0 + 4)))[i] = obj;
            int k = ((int *)(*(int *)(field_b0 + 4)))[i];
            Player_setRadius(*(int *)(k + 4), 800);
            Player_setAlwaysFriend(*(int *)(k + 4), 1);
            Player_setMaxHitpoints(*(int *)(k + 4), 100);
            (*(void (**)(int, int, int, int))(*(int *)k + 0x48))(k, color, color, color);
            KIPlayer_setActive(k, 0);
            ArrayAdd(k, enemies);
        }
    }
}
