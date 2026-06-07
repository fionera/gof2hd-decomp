#include "class.h"

struct Status;
struct Ship;
struct Item;

extern "C" int Status_getCurrentCampaignMission(Status *s);
extern "C" Ship *Status_getShip(Status *s);
extern "C" unsigned *Ship_getEquipment(Ship *ship);
extern "C" int Item_getType(Item *it);

// Status singleton holder: pc-rel -> holder; *holder is the Status object.
extern void *const gAchStatusHolder __attribute__((visibility("hidden")));

extern "C" void Achievements_initCheckEquipmentAndWeapons(Achievements *self)
{
    uint8_t result;
    if (Status_getCurrentCampaignMission(*(Status **)gAchStatusHolder) < 8) {
        result = 1;
    } else {
        Ship *ship = Status_getShip(*(Status **)gAchStatusHolder);
        unsigned *eq = Ship_getEquipment(ship);
        int weapons = 0;
        int turrets = 0;
        if (eq != 0) {
            for (unsigned i = 0; i < *eq; i = i + 1) {
                Item *it = ((Item **)eq[1])[i];
                if (it != 0 && Item_getType(it) != 4) {
                    if (Item_getType(((Item **)eq[1])[i]) == 0) {
                        F<int>(self, 0x14) += 1;
                    } else if (Item_getType(((Item **)eq[1])[i]) == 3) {
                        turrets = turrets + 1;
                        continue;
                    }
                    weapons = weapons + 1;
                }
            }
        }
        result = (0 < turrets) & (0 < weapons);
    }
    F<uint8_t>(self, 0x18) = result;
}
