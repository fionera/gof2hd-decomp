#include "class.h"

struct Status;
struct Ship;
struct PlayerEgo;
__attribute__((visibility("hidden"))) extern Status **g_status;
extern "C" Ship *Status_getShip(Status *s);
extern "C" int Ship_getFirstEquipmentOfSort(Ship *ship, int sort);
extern "C" int PlayerEgo_hasAutoTurret(PlayerEgo *p);
extern "C" int PlayerEgo_isDockedToDockingPoint(PlayerEgo *p);

// MGame::nextCamId(int): advances through the cycle of available camera modes, skipping
// the turret cam unless the ship actually has a turret, and the docked cam unless docked.
extern "C" int MGame_nextCamId(MGame *self, int cur) {
    int id = cur + 1;
    if (id == 2) id = cur + 2;
    if (id == 1) {
        Status **sp = g_status;
        if (Ship_getFirstEquipmentOfSort(Status_getShip(*sp), 8) != 0 ||
            Ship_getFirstEquipmentOfSort(Status_getShip(*sp), 0x23) != 0) {
            id = PlayerEgo_hasAutoTurret(F<PlayerEgo *>(self, 0x58)) == 0 ? 1 : 2;
        } else {
            id = 2;
        }
    }
    if (id == 2) id = 3;
    if (id >= 4) {
        if (PlayerEgo_isDockedToDockingPoint(F<PlayerEgo *>(self, 0x58)) == 0) return 0;
        Status **sp = g_status;
        if (Ship_getFirstEquipmentOfSort(Status_getShip(*sp), 8) == 0 &&
            Ship_getFirstEquipmentOfSort(Status_getShip(*sp), 0x23) == 0) {
            return 3;
        }
        id = PlayerEgo_hasAutoTurret(F<PlayerEgo *>(self, 0x58)) != 0 ? 3 : 1;
    }
    return id;
}
