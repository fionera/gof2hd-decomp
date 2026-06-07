#include "class.h"

struct Status;
struct Station;
struct Ship;
struct Item;
struct Achievements;

// Status singleton holder: g -> P, P[0] -> Status object.
__attribute__((visibility("hidden"))) extern Status **g_ModStation_es_status;
// Achievements holder-of-holder.
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_es_ach;
// getShip veneer accessed via a function-pointer global.
__attribute__((visibility("hidden"))) extern Ship *(*g_ModStation_es_getShip)(Status *);

extern "C" Station *Status_getStation(Status *s);
extern "C" void Status_departStation(Status *s, Station *st);
extern "C" void Station_visit(Station *st);
extern "C" void Achievements_applyNewMedals(Achievements *a);
extern "C" Item *Ship_getFirstEquipmentOfSort(Ship *ship, int sort);
extern "C" int Ship_getIndex(Ship *ship);
extern "C" int Item_getIndex(Item *it);

// ModStation::enterStation()
extern "C" void ModStation_enterStation()
{
    Status **holder = g_ModStation_es_status;
    Status *status = holder[0];
    Status_departStation(status, Status_getStation(status));
    Station_visit(Status_getStation(holder[0]));
    Achievements_applyNewMedals(g_ModStation_es_ach[0]);

    Ship *(*getShip)(Status *) = g_ModStation_es_getShip;
    Item *e10 = Ship_getFirstEquipmentOfSort(getShip(holder[0]), 10);
    Item *e9 = Ship_getFirstEquipmentOfSort(getShip(holder[0]), 9);
    int shipIdx = Ship_getIndex(getShip(holder[0]));
    Status *s = holder[0];
    I(s, 0x150) = shipIdx;

    int v;
    if (e10 == 0) {
        v = -1;
    } else {
        v = Item_getIndex(e10);
        s = holder[0];
    }
    I(s, 0x154) = v;

    if (e9 == 0) {
        v = -1;
    } else {
        v = Item_getIndex(e9);
        s = holder[0];
    }
    I(s, 0x158) = v;
    I(s, 0x124) = 0;
    I(s, 0x11c) = 0;
}
