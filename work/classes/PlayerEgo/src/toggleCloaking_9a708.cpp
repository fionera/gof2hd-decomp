#include "class.h"

// PlayerEgo::toggleCloaking()
//   Engages or disengages the cloaking device. When uncloaked and chargeable
//   (and carrying enough plasma, item 0x7a), it consumes the plasma, sets the
//   cloak flag and fires the two HUD events. When already cloaked and the cloak
//   timer has run out, it switches the hull (and, for ships with secondary
//   meshes, the wing meshes) to the cloak material 0xe, retunes their shader
//   anim values, and -- for the appropriate ship variant -- creates and binds
//   the resource material.

extern "C" int   Item_getAttribute(void *item);
extern "C" int   Item_getAmount(void *item);
extern "C" int   Item_getIndex(void *item);
extern "C" void *Status_getShip();
extern "C" void *Ship_getCargo(void *ship, int item);
extern "C" void  Ship_removeCargo(void *ship, int item);
extern "C" int   Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" void  Hud_hudEvent(void *hud, int ev, void *arg);
extern "C" int   FModSound_play(int snd, int id, void *a, int b);
extern "C" void *PaintCanvas_MaterialGetMaterial(void *canvas, int handle);
extern "C" void  PaintCanvas_MeshChangeMaterial(void *canvas, unsigned int mesh, unsigned short mat);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, unsigned int mesh);
extern "C" void  PaintCanvas_MeshChangeShaderAnimValue(void *canvas, void *mesh, float v, int i);
extern "C" void  PaintCanvas_MaterialCreate(void *canvas, unsigned short id, unsigned int *out);
extern "C" void  PaintCanvas_MeshChangeResourceMaterial(void *canvas, unsigned int mesh,
                                                        unsigned short mat);

__attribute__((visibility("hidden"))) extern int  *g_PE_tc_sound;     // 0xaa786 cloak sound
__attribute__((visibility("hidden"))) extern void **g_PE_tc_canvas;   // 0xaa798 PaintCanvas

extern "C" void PlayerEgo_toggleCloaking(PlayerEgo *self)
{
    if (C(self, 0x1ad) == 0) {
        // currently uncloaked -> try to cloak
        if (C(self, 0x1ac) != 0 || I(self, 0x20c) > 0)
            return;
        int need = Item_getAttribute(P(self, 0x1b0));
        void *cargo = Ship_getCargo(Status_getShip(), 0x7a);
        int have = (cargo == 0) ? 0 : Item_getAmount(cargo);
        if (need <= have) {
            Ship_removeCargo(Status_getShip(), 0x7a);
            C(self, 0x1ad) = 1;
            Hud_hudEvent(P(self, 0x220), 0x1e, self);
            Hud_hudEvent(P(self, 0x220), 0x1c, self);
        }
        return;
    }

    // currently cloaked -> uncloak once the cloak hold timer elapses
    if (I(self, 0x214) > I(self, 0x208))
        return;

    FModSound_play(*g_PE_tc_sound, 0x1e, 0, 0);
    void *canvas = *g_PE_tc_canvas;
    C(P(self, 0x0), 0x5e) = 1;
    I(self, 0x208) = 0;
    C(self, 0x1ac) = 1;

    PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x388));     // returned ptr +0x20 = 0xe below
    I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x388)), 0x20) = 0xe;
    PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c), *(unsigned short*)((char*)self + 0x388));
    PaintCanvas_MeshChangeShaderAnimValue(canvas,
        PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c)), 0.0f, 0);
    PaintCanvas_MeshChangeShaderAnimValue(canvas,
        PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x4) + 0x1c)), 0.0f, 0);

    if (C(self, 0x170) != 0) {
        I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x38c)), 0x20) = 0xe;
        I(PaintCanvas_MaterialGetMaterial(canvas, I(self, 0x390)), 0x20) = 0xe;
        PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c), *(unsigned short*)((char*)self + 0x38c));
        PaintCanvas_MeshChangeMaterial(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c), *(unsigned short*)((char*)self + 0x390));

        void *m;
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 1);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 2);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 1);
        m = PaintCanvas_MeshGetPointer(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c));
        PaintCanvas_MeshChangeShaderAnimValue(canvas, m, 0.0f, 2);

        if (C(self, 0x170) != 0) {
            unsigned short mat = 0x4e8e;
            void *it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 8);
            if (it != 0 && Item_getIndex(it) == 0xe0)
                mat = 0x5e17;
            it = (void *)Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23);
            if (it != 0) {
                int idx = Item_getIndex(it);
                mat = 0x716d;
                if (idx == 0xc7) mat = 0x7167;
                if (idx == 0xc6) mat = 0x7161;
            }
            unsigned int out;
            PaintCanvas_MaterialCreate(canvas, mat, &out);
            PaintCanvas_MeshChangeResourceMaterial(canvas, *(unsigned int *)(I(self, 0xdc) + 0x1c), mat);
            PaintCanvas_MeshChangeResourceMaterial(canvas, *(unsigned int *)(I(self, 0x28) + 0x1c), mat);
        }
    }
}
