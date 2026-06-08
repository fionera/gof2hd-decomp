#include "class.h"

// PlayerEgo::setShip(int race, int group)
//   Rebuilds the player's ship object and all of its attached hardware:
//     * fetches the ship group prototype (stored at 0x4) and its hull mesh,
//     * creates the hull AEGeometry (0x8) and parents it,
//     * if a tractor beam is equipped, builds it (0x1b4) and registers sounds,
//     * loops the two repair-beam equipment sorts, building a RepairBeam list
//       (0x1b8) and registering their sounds,
//     * if an emergency system is fitted, builds its effect geometry (0xac) and
//       caches its transform-derived values (0x310/0x314/0x320),
//     * applies supernova / turret-specific scaling (0x3c, tail veneer 0x1ab9f8).

extern "C" void *Globals_getShipGroup(void *g, int race, int group, bool b);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, int meshId);
extern "C" void *AEGeometry_new(void *canvas);
extern "C" void  AEGeometry_addChild(void *parent, void *child);
extern "C" void *Status_getShip();
extern "C" int   Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" int   Ship_hasEmergencySystem(void *ship);
extern "C" int   Item_getIndex(void *item);
extern "C" int   Item_getSort(void *item);
extern "C" int   Item_getAttribute(void *item);
extern "C" void *TractorBeam_new(void *geo, int kind);
extern "C" void  Globals_addSoundResourceToList(int snd);
extern "C" void *RepairBeam_new(int idx, int sort);
extern "C" void  ArrayAdd_RepairBeam(void *rb, void *arr);
extern "C" void *Array_RepairBeam_new();
extern "C" void *AEGeometry_newMesh(int meshId, void *canvas, bool b);    // 0x7207c
extern "C" void *PaintCanvas_TransformGetTransform(unsigned int handle);
extern "C" void  Vec_assign(void *dst, const void *src);
extern "C" int   Status_inSupernovaSystem();
extern "C" int   Status_inSupernovaOrbit();
extern "C" void  PlayerEgo_setShip_tail(void *canvas, int meshId, void *out, void **canvasHolder);

__attribute__((visibility("hidden"))) extern void **g_PE_ss_globals;  // 0xaa286 Globals singleton
__attribute__((visibility("hidden"))) extern void **g_PE_ss_canvas;   // 0xaa29a PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_ss_status;   // 0xaa2ca Status singleton
extern const float g_PE_ss_emDiv;     // 0xaa4b8
extern const float g_PE_ss_emBias;    // 0xaa4bc

extern "C" void PlayerEgo_setShip(PlayerEgo *self, int race, int group)
{
    void **globals = g_PE_ss_globals;
    void *grp = Globals_getShipGroup(*globals, race, group, true);
    P(self, 0x4) = grp;

    void **canvasHolder = g_PE_ss_canvas;
    void *canvas = *canvasHolder;
    void *mesh = PaintCanvas_MeshGetPointer(canvas, I(grp, 0x1c));
    P(self, 0x394) = *(void **)(I(mesh, 0x30) + 0x20);

    void *hull = AEGeometry_new(*canvasHolder);
    P(self, 0x8) = hull;
    AEGeometry_addChild(hull, *(void **)((char *)P(self, 0x4) + 0xc));

    // tractor beam
    if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0xd) != 0) {
        void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0xd);
        int idx = Item_getIndex(it);
        int kind = (idx < 0x48) ? idx - 0x44 : 3;
        void *tb = TractorBeam_new(P(self, 0x8), kind);
        P(self, 0x1b4) = tb;
        Globals_addSoundResourceToList(*(int *)(*globals));
        Globals_addSoundResourceToList(*(int *)(*globals));
    }

    // repair beams (sorts 0x25 and 0x29)
    for (unsigned i = 0; i < 2; i++) {
        int sort = (i == 0) ? 0x25 : 0x29;
        void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), sort);
        if (it != 0) {
            if (P(self, 0x1b8) == 0)
                P(self, 0x1b8) = Array_RepairBeam_new();
            void *rb = RepairBeam_new(Item_getIndex(it), Item_getSort(it));
            int idx = Item_getIndex(it);
            if (idx == 0xde)
                Globals_addSoundResourceToList(*(int *)(*globals));
            else if (Item_getIndex(it) == 0xdf)
                Globals_addSoundResourceToList(*(int *)(*globals));
            ArrayAdd_RepairBeam(rb, P(self, 0x1b8));
        }
    }

    // emergency system effect geometry
    if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0x1b) != 0
        && Ship_hasEmergencySystem(Status_getShip()) != 0) {
        void *geo = AEGeometry_newMesh(0x3826, *canvasHolder, false);
        P(self, 0xac) = geo;
        Ship_getFirstEquipmentOfSort(Status_getShip(), 0x1b);
        I(self, 0x310) = Item_getAttribute((void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0x29));
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        Vec_assign((char *)self + 0x314, (char *)tf + 0xd4);
        tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        F(self, 0x320) = F(tf, 0xe0) / g_PE_ss_emDiv + g_PE_ss_emBias;
    }

    // supernova scaling
    if (Status_inSupernovaSystem() != 0 || Status_inSupernovaOrbit() != 0) {
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)((char *)P(self, 0x4) + 0xc));
        F(self, 0x3c) = F(tf, 0xe0) * 1.75f;
    }

    // turret offset finalisation
    if (P(self, 0x1b0) != 0)
        PlayerEgo_setShip_tail(*canvasHolder, I(P(self, 0x4), 0x1c),
                               (char *)self + 0x388, canvasHolder);
}
