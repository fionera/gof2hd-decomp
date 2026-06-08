#include "class.h"

// RepairBeam::RepairBeam(int shipIndex, int sort) — builds the per-target geometry/id/charge
// arrays sized to the player's equipment-of-sort attribute count.
extern "C" int RepairBeam_Status_getShip();
extern "C" int RepairBeam_Ship_getFirstEquipmentOfSort(int ship);
extern "C" int RepairBeam_Item_getAttribute(int item);
extern "C" void RepairBeam_Array_Geo_ctor(void *arr);
extern "C" void RepairBeam_Array_int_ctor(void *arr);
extern "C" void RepairBeam_Array_float_ctor(void *arr);
extern "C" void RepairBeam_ArraySetLength_Geo(int n, void *arr);
extern "C" void RepairBeam_ArraySetLength_int(int n, void *arr);
extern "C" void RepairBeam_ArraySetLength_float(int n, void *arr);
extern "C" void RepairBeam_AEGeometry_ctor(void *geom, unsigned short id, void *canvas, bool b);
extern "C" __attribute__((visibility("hidden"))) void **g_RepairBeam_canvas;

extern "C" RepairBeam *RepairBeam_ctor(RepairBeam *self, int shipIndex, int sort)
{
    F<int>(self, 0x1c) = sort;
    F<int>(self, 0x00) = shipIndex;
    F<int>(self, 0x04) = 0;
    F<int>(self, 0x08) = 0;
    F<int>(self, 0x0c) = 0;

    int ship = RepairBeam_Status_getShip();
    int equip = RepairBeam_Ship_getFirstEquipmentOfSort(ship);
    int count = RepairBeam_Item_getAttribute(equip);

    // geometry array
    void *geoms = operator new(0xc);
    RepairBeam_Array_Geo_ctor(geoms);
    F<void *>(self, 0x10) = geoms;
    RepairBeam_ArraySetLength_Geo(count, geoms);

    unsigned short geoId = 0x4a95;
    if (sort == 0x25) {
        geoId = 0x4a94;
    }
    void **canvas = g_RepairBeam_canvas;
    for (int i = 0; i < count; i = i + 1) {
        void *geo = operator new(0xc0);
        RepairBeam_AEGeometry_ctor(geo, geoId, *canvas, false);
        ((void **)F<void **>(self, 0x10)[1])[i] = geo;
    }

    // target-id array
    void *ids = operator new(0xc);
    RepairBeam_Array_int_ctor(ids);
    F<void *>(self, 0x14) = ids;
    RepairBeam_ArraySetLength_int(count, ids);
    F<int>(self, 0x20) = 0x9c4;
    {
        unsigned *arr = F<unsigned *>(self, 0x14);
        for (unsigned k = 0; k < arr[0]; k = k + 1) {
            ((int *)arr[1])[k] = -1;
        }
    }

    // charge array
    void *charges = operator new(0xc);
    RepairBeam_Array_float_ctor(charges);
    F<void *>(self, 0x18) = charges;
    RepairBeam_ArraySetLength_float(count, charges);
    {
        int *arr = F<int *>(self, 0x18);
        int off = 0;
        for (int j = arr[0]; j != 0; j = j - 1) {
            *(int *)((char *)arr[1] + off) = 0;
            off = off + 4;
        }
    }

    return self;
}
