#ifndef GOF2_REPAIRBEAM_H
#define GOF2_REPAIRBEAM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// RepairBeam — top-level class (only arg types are namespaced).
// Layout recovered from ctor/dtor/render/update decompiles:
//   +0x00  int    shipIndex      (used to index a sound-event global array)
//   +0x04  Vector beamPosition   (x@+0x4, y@+0x8, z@+0xc)
//   +0x10  Array<AEGeometry*>* geoms
//   +0x14  Array<int>*           targetIds   (size@[0], data@[1])
//   +0x18  Array<float>*         charges     (size@[0], data@[1])
//   +0x1c  int    sort           (0x25 heal / 0x29 shield)
//   +0x20  int    timer

void *operator new(__SIZE_TYPE__ size);

struct RepairBeam;

namespace AbyssEngine {
namespace AEMath {

}
}
using Vector = AbyssEngine::AEMath::Vector;

class RepairBeam {
public:
    int shipIndex;                      // +0x0   ship index (sound-event array index)
    Vector beamPosition;                // +0x4   beam endpoint (x@+0x4, y@+0x8, z@+0xc)
    void** geometries;                  // +0x10  Array<AEGeometry*>* (legacy [0]=size,[1]=data)
    void** targetIds;                   // +0x14  Array<int>*   (target enemy slot, -1 = empty)
    void** charges;                     // +0x18  Array<float>* (per-target charge accumulator)
    int sort;                           // +0x1c  equipment sort (0x25 heal / 0x29 shield)
    int timer;                          // +0x20  re-arm countdown timer

    // ---- constructor / destructor (demangle to RepairBeam::RepairBeam / ~RepairBeam) ----
    RepairBeam(int shipIndex, int sort);
    ~RepairBeam();
    void render();
    void update(int dt, void *level, void *hud);

    // Heap factory: allocate + construct a RepairBeam for the given ship index and
    // equipment sort (0x25 heal / 0x29 shield). Used by PlayerEgo when a repair-
    // beam module is equipped.
    static RepairBeam *create(int shipIndex, int sort);

    // ---- legacy 0xc-byte Array<T> helpers (size@+0, data@+4, capacity@+8) -------
    // These mirror the engine's templated Array<T> instantiations that the ctor/
    // dtor build by hand for the per-target geometry / id / charge arrays. They are
    // kept as static members so each typed slot reads clearly at the call site.
    static void arrayGeoCtor(void *arr);
    static void arrayIntCtor(void *arr);
    static void arrayFloatCtor(void *arr);
    static void arraySetLengthGeo(int n, void *arr);
    static void arraySetLengthInt(int n, void *arr);
    static void arraySetLengthFloat(int n, void *arr);
    static void arrayReleaseClassesGeo(void *arr);
    static void *arrayGeoDtor(void *arr);
    static void *arrayIntDtor(void *arr);
    static void *arrayFloatDtor(void *arr);

    // ---- cross-class forwarders the ctor/update use ---------------------------
    // The active player ship, its first equipment of the configured sort, and that
    // item's primary attribute (the beam range / target count).
    static int playerShip();
    static int shipFirstEquipmentOfSort(int ship);
    static int itemAttribute(int item);
};
#endif
