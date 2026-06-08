#include "class.h"

// PlayerEgo::checkForTurret()
//   Lazily builds the gun-turret model the first time a turret weapon is
//   available. It reads the equipped turret (slot 2), derives its tracking range
//   (0x1f8), then -- via a big switch on the turret item index -- selects the
//   base / barrel / muzzle / extra mesh ids, instantiates the AEGeometry chain
//   (0xf4 base, 0x34 yaw, 0x38 muzzle root, plus optional children), tunes the
//   muzzle transform, positions everything at the hardpoint, and (for AI-driven
//   players) clones the materials. The placement rotate/translate math is
//   delegated to PE_cft_place.

extern "C" int   Player_gunAvailable(void *player);
extern "C" void *Status_getShip();
extern "C" int   Ship_getEquipment(void *ship, int slot);
extern "C" int   Item_getAttribute(void *item);
extern "C" int   Item_getIndex(void *item);
extern "C" void *AEGeometry_newMesh(unsigned short mesh, void *canvas, bool b);
extern "C" void *AEGeometry_new(void *canvas);
extern "C" void  AEGeometry_setRotationOrder(void *geo, int order);
extern "C" void  AEGeometry_addChild(void *parent, void *child);
extern "C" void *AEGeometry_dtor(void *geo);
extern "C" void  AEGeometry_setPosition(void *geo, const void *vec);
extern "C" void  operator_delete_(void *p);
extern "C" void *PaintCanvas_TransformGetTransform(unsigned int handle);
extern "C" void  Transform_SetVisible(void *tf, bool vis);
extern "C" void  Transform_SetAnimationState(void *tf, int a, int b);
extern "C" void  PaintCanvas_MeshCloneMaterial(void *canvas, int mesh, unsigned int *out);
extern "C" void  PE_cft_finishMaterials(void *canvas, int mesh, void *out);   // 0x1aba08 veneer
extern "C" void  PE_cft_place(PlayerEgo *self, int turretIdx);                // placement math

__attribute__((visibility("hidden"))) extern void **g_PE_cft_canvas;   // 0xab512 PaintCanvas
extern const float g_PE_cft_transformVal;   // 0xab76c muzzle transform +0xe0

extern "C" void PlayerEgo_checkForTurret(PlayerEgo *self)
{
    if (C(self, 0x170) != 0)
        return;
    int avail = Player_gunAvailable(P(self, 0x0));
    C(self, 0x170) = (unsigned char)avail;
    if (avail == 0)
        return;

    C(self, 0x180) = 0;
    int equip = Ship_getEquipment(Status_getShip(), 2);
    void *item = *(void **)(equip + 4);
    I(self, 0x1f8) = (int)((double)Item_getAttribute(*(void **)(item)) * 1.5);

    int idx = Item_getIndex(*(void **)(item));
    unsigned short base = 0xffff, barrel = 0xffff;
    int muzzle = -1, child = -1, extra = -1, extra2 = -1;

    if (idx == 0xe0) {
        base = 0x499a; barrel = 0x499b; muzzle = 0x499c; child = 0x499d;
    } else if (idx == 0x30) {
        base = 0x1a74; barrel = 0x1a75;
    } else if (idx == 0x31) {
        base = 0x1a76; barrel = 0x1a77;
    } else if (idx == 0xb4) {
        C(self, 0x180) = 1; base = 0x1a95; barrel = 0x1a96;
    } else if (idx == 0xb5) {
        C(self, 0x180) = 1; base = 0x1a97; barrel = 0x1a98;
    } else if (idx == 0xb6) {
        C(self, 0x180) = 1; base = 0x1a99; barrel = 0x1a9a;
    } else if (idx == 0xc6) {
        base = 0x4963; barrel = 0x4964; muzzle = 0x4966; child = 0x4967; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc7) {
        base = 0x4968; barrel = 0x4969; muzzle = 0x496a; child = 0x496b; extra = -1; extra2 = 0x4a7f;
    } else if (idx == 0xc8) {
        base = 0x496c; barrel = 0x496d; muzzle = 0x496e; child = 0x496f; extra = 0x4970; extra2 = 0x4a7f;
    } else if (idx == 0x2f) {
        base = 0x1a72; barrel = 0x1a73;
    }

    void **canvasHolder = g_PE_cft_canvas;
    void *canvas = *canvasHolder;

    void *baseGeo = AEGeometry_newMesh(base, canvas, false);
    P(self, 0xf4) = baseGeo;
    void *yawGeo = AEGeometry_newMesh(barrel, canvas, false);
    P(self, 0x34) = yawGeo;
    AEGeometry_setRotationOrder(yawGeo, 2);
    void *muzzleRoot = AEGeometry_new(canvas);
    P(self, 0x38) = muzzleRoot;

    if (muzzle != -1) {
        void *g = AEGeometry_newMesh((unsigned short)muzzle, canvas, false);
        AEGeometry_addChild(P(self, 0xf4), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (child != -1) {
        void *g = AEGeometry_newMesh((unsigned short)child, canvas, false);
        AEGeometry_addChild(P(self, 0x34), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (extra != -1) {
        void *g = AEGeometry_newMesh((unsigned short)extra, canvas, false);
        AEGeometry_addChild(P(self, 0x34), g);
        operator_delete_(AEGeometry_dtor(g));
    }
    if (extra2 != -1) {
        void *g = AEGeometry_newMesh((unsigned short)extra2, canvas, false);
        P(self, 0x3c) = g;
        AEGeometry_addChild(P(self, 0x34), g);
        void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
        Transform_SetVisible(tf, C(self, 0x2c4) != 0);
    }

    void *tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
    F(tf, 0xe0) = g_PE_cft_transformVal;
    tf = PaintCanvas_TransformGetTransform(*(unsigned int *)canvasHolder);
    Transform_SetAnimationState(tf, 2, 0);

    AEGeometry_setPosition(P(self, 0xf4), (char *)self + 0x23c);
    AEGeometry_setPosition(P(self, 0x34), (char *)self + 0x23c);

    PE_cft_place(self, idx);

    AEGeometry_addChild(P(self, 0x38), P(self, 0xf4));
    AEGeometry_addChild(P(self, 0x38), P(self, 0x34));

    if (P(self, 0x2c0) != 0 && C(self, 0x170) != 0) {
        PaintCanvas_MeshCloneMaterial(canvas, I(P(self, 0xf4), 0x1c), &U(self, 0x388));
        PE_cft_finishMaterials(canvas, I(P(self, 0x34), 0x1c), (char *)self + 0x38c);
    }
}
