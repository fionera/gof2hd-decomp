#include "class.h"

extern "C" void operator_delete(void *p);
extern "C" void RecordHandler_saveOptions(void *rh);
extern "C" void *Galaxy_dtor(void *g);
extern "C" void *Status_dtor(void *s);
extern "C" void *GameText_dtor(void *g);
extern "C" void *AERandom_dtor(void *r);
extern "C" void *Layout_dtor(void *l);
extern "C" void *Generator_dtor(void *g);
extern "C" void *RecordHandler_dtor(void *rh);
extern "C" void *FModSound_dtor(void *s);
extern "C" void *Achievements_dtor(void *a);
extern "C" void *ImageFactory_dtor(void *f);
extern "C" void ArrayReleaseClasses_Item(void *a);
extern "C" void *ArrayItem_dtor(void *a);
extern "C" void ArrayReleaseClasses_Ship(void *a);
extern "C" void *ArrayShip_dtor(void *a);
extern "C" void ArrayRelease_int(void *a);
extern "C" void *ArrayInt_dtor(void *a);

// Each singleton lives behind a hidden PC-relative pointer-to-pointer global.
extern void **const gG_recordHandler __attribute__((visibility("hidden")));  // DAT_000f35e0
extern void **const gG_galaxy __attribute__((visibility("hidden")));         // DAT_000f35e4
extern void **const gG_status __attribute__((visibility("hidden")));         // DAT_000f35e8
extern void **const gG_gameText __attribute__((visibility("hidden")));       // DAT_000f35ec
extern void **const gG_random __attribute__((visibility("hidden")));         // DAT_000f35f0
extern void **const gG_layout __attribute__((visibility("hidden")));         // DAT_000f35f4
extern void **const gG_generator __attribute__((visibility("hidden")));      // DAT_000f35f8
extern void **const gG_polyObj __attribute__((visibility("hidden")));        // DAT_000f35fc (vtable dtor)
extern void **const gG_fmod __attribute__((visibility("hidden")));           // DAT_000f3600
extern void **const gG_items __attribute__((visibility("hidden")));          // DAT_000f3604
extern void **const gG_ships __attribute__((visibility("hidden")));          // DAT_000f3608
extern void **const gG_achievements __attribute__((visibility("hidden")));   // DAT_000f360c
extern void **const gG_imageFactory __attribute__((visibility("hidden")));   // DAT_000f3610
extern int **const gG_tail __attribute__((visibility("hidden")));            // DAT_000f3614

// Globals::~Globals()
extern "C" void *Globals_dtor(Globals *self)
{
    void **rhSlot = gG_recordHandler;
    if (*rhSlot != 0) {
        RecordHandler_saveOptions(*rhSlot);
    }
    void **galSlot = gG_galaxy;
    if (*galSlot != 0) {
        operator_delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    void **statSlot = gG_status;
    if (*statSlot != 0) {
        operator_delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    void **gtSlot = gG_gameText;
    if (*gtSlot != 0) {
        operator_delete(GameText_dtor(*gtSlot));
    }
    *gtSlot = 0;
    void **rngSlot = gG_random;
    if (*rngSlot != 0) {
        operator_delete(AERandom_dtor(*rngSlot));
    }
    *rngSlot = 0;
    void **laySlot = gG_layout;
    if (*laySlot != 0) {
        operator_delete(Layout_dtor(*laySlot));
    }
    *laySlot = 0;
    void **genSlot = gG_generator;
    if (*genSlot != 0) {
        operator_delete(Generator_dtor(*genSlot));
    }
    *genSlot = 0;
    if (*rhSlot != 0) {
        operator_delete(RecordHandler_dtor(*rhSlot));
    }
    *rhSlot = 0;
    void **polySlot = gG_polyObj;
    void *poly = *polySlot;
    if (poly != 0) {
        void (**vt)(void *) = *(void (***)(void *))poly;
        vt[1](poly);
    }
    *polySlot = 0;
    void **fmodSlot = gG_fmod;
    if (*fmodSlot != 0) {
        operator_delete(FModSound_dtor(*fmodSlot));
    }
    *fmodSlot = 0;
    void **itemSlot = gG_items;
    if (*itemSlot != 0) {
        ArrayReleaseClasses_Item(*itemSlot);
        if (*itemSlot != 0) {
            operator_delete(ArrayItem_dtor(*itemSlot));
        }
    }
    *itemSlot = 0;
    void **shipSlot = gG_ships;
    if (*shipSlot != 0) {
        ArrayReleaseClasses_Ship(*shipSlot);
        if (*shipSlot != 0) {
            operator_delete(ArrayShip_dtor(*shipSlot));
        }
    }
    *shipSlot = 0;
    if (*galSlot != 0) {
        operator_delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    void **achSlot = gG_achievements;
    if (*achSlot != 0) {
        operator_delete(Achievements_dtor(*achSlot));
    }
    *achSlot = 0;
    if (*statSlot != 0) {
        operator_delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    void **ifSlot = gG_imageFactory;
    if (*ifSlot != 0) {
        operator_delete(ImageFactory_dtor(*ifSlot));
    }
    *ifSlot = 0;

    void *selfArr = F<void *>(self, 4);
    if (selfArr != 0) {
        ArrayRelease_int(selfArr);
        if (F<void *>(self, 4) != 0) {
            operator_delete(ArrayInt_dtor(F<void *>(self, 4)));
        }
    }
    F<void *>(self, 4) = 0;
    **gG_tail = 0;
    return self;
}
