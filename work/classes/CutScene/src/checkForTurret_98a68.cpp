#include "class.h"

extern "C" {
void PaintCanvas_TransformRemoveChild(void *canvas, unsigned int parent, unsigned int child);
void *Status_getShip();
int *Ship_getEquipment(void *ship, int slot);
int Item_getIndex(void *item);
void *operator_new(unsigned int size);
void operator_delete(void *p);
void AEGeometry_ctor_id(void *self, unsigned short id, void *canvas, bool flag);
void AEGeometry_ctor_default(void *self, void *canvas);
void AEGeometry_setRotationOrder(void *self, int order);
void AEGeometry_addChild(void *self, unsigned int child);
void *AEGeometry_dtor(void *self);
void AEGeometry_setPosition(void *self, void *vec);
void AEGeometry_translate(float x, float y, float z);
void AEGeometry_rotate(float x, float y, float z);
void FileRead_ctor(void *self);
void *FileRead_dtor(void *self);
void *FileRead_loadWeaponPositions(void *self, int shipIndex);
int Ship_getIndex(void *ship);
void *Level_getEnemies(void *level);
void ArrayReleaseClasses_VectorPtr(void *arr);
void *Array_VectorPtr_dtor(void *arr);
void ArrayReleaseClasses_ArrayVectorPtr(void *arr);
void *Array_ArrayVectorPtr_dtor(void *arr);
void CutScene_turretFinalize();
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
extern "C" {
extern float CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz;
extern float CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz;
extern float CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2;
}

void CutScene::checkForTurret()
{
    if (pp(this, 0x64) != 0) {
        void *canvas = *g_canvas;
        void *enemies = Level_getEnemies(pp(this, 0x0));
        unsigned int parent = u32(*(void **)(*(char **)((char *)enemies + 4) + 8), 0xc);
        unsigned int child = u32(pp(this, 0x64), 0xc);
        PaintCanvas_TransformRemoveChild(canvas, parent, child);
    }

    void *ship = Status_getShip();
    int *equip = Ship_getEquipment(ship, 2);
    if (equip == 0 || equip[0] == 0)
        return;
    void *item = *(void **)(equip[1]);
    if (item == 0)
        return;

    int idx = Item_getIndex(item);

    unsigned short id0 = 0xffff;   // base geometry
    unsigned short id1 = 0xffff;   // second geometry
    int child0 = -1;
    int child1 = -1;
    int child2 = -1;

    switch (idx) {
    case 0xe0:
        id0 = 0x499a; id1 = 0x499b; child0 = 0x499c; child1 = 0x499d; child2 = -1; break;
    case 0x30: id0 = 0x1a74; id1 = 0x1a75; break;
    case 0x31: id0 = 0x1a76; id1 = 0x1a77; break;
    case 0xb4: id0 = 0x1a95; id1 = 0x1a96; break;
    case 0xb5: id0 = 0x1a97; id1 = 0x1a98; break;
    case 0xb6: id0 = 0x1a99; id1 = 0x1a9a; break;
    case 0xc6:
        id0 = 0x4963; id1 = 0x4967; child0 = 0x4964; child1 = 0x4966; child2 = -1; break;
    case 199:
        id0 = 0x4968; id1 = 0x496b; child0 = 0x4969; child1 = 0x496a; child2 = -1; break;
    case 200:
        id0 = 0x496c; id1 = 0x496f; child0 = 0x496d; child1 = 0x496e; child2 = 0x4970; break;
    case 0x2f: id0 = 0x1a72; id1 = 0x1a73; break;
    default:   id0 = 0xffff; id1 = 0xffff; break;
    }

    void *canvas = *g_canvas;

    void *geom0 = operator_new(0xc0);
    AEGeometry_ctor_id(geom0, id0, canvas, false);

    void *geom1 = operator_new(0xc0);
    AEGeometry_ctor_id(geom1, id1, canvas, false);
    AEGeometry_setRotationOrder(geom1, 2);

    if (child0 != -1) {
        void *c = operator_new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child0, canvas, false);
        AEGeometry_addChild(geom0, u32(c, 0xc));
        operator_delete(AEGeometry_dtor(c));
    }
    if (child1 != -1) {
        void *c = operator_new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child1, canvas, false);
        AEGeometry_addChild(geom1, u32(c, 0xc));
        operator_delete(AEGeometry_dtor(c));
    }
    if (child2 != -1) {
        void *c = operator_new(0xc0);
        AEGeometry_ctor_id(c, (unsigned short)child2, canvas, false);
        AEGeometry_addChild(geom1, u32(c, 0xc));
        operator_delete(AEGeometry_dtor(c));
    }

    if (pp(this, 0x64) != 0)
        operator_delete(AEGeometry_dtor(pp(this, 0x64)));
    pp(this, 0x64) = 0;

    void *root = operator_new(0xc0);
    AEGeometry_ctor_default(root, canvas);
    pp(this, 0x64) = root;

    void *fr = operator_new(1);
    FileRead_ctor(fr);
    Status_getShip();
    int shipIdx = Ship_getIndex(Status_getShip());
    void *positions = FileRead_loadWeaponPositions(fr, shipIdx);
    operator_delete(FileRead_dtor(fr));

    void *posVec = *(void **)(*(char **)(*(char **)((char *)positions + 4) + 8) + 4);
    AEGeometry_setPosition(geom0, posVec);
    AEGeometry_setPosition(geom1, posVec);
    AEGeometry_translate(CutScene_turret_tx, CutScene_turret_ty, CutScene_turret_tz);

    int idx2 = Item_getIndex(item);
    if (idx2 >= 0xc6 && Item_getIndex(item) <= 200) {
        AEGeometry_rotate(CutScene_turret_rx, CutScene_turret_ry, CutScene_turret_rz);
        AEGeometry_rotate(CutScene_turret_rx2, CutScene_turret_ry2, CutScene_turret_rz2);
    }

    AEGeometry_addChild(pp(this, 0x64), u32(geom0, 0xc));
    AEGeometry_addChild(pp(this, 0x64), u32(geom1, 0xc));
    void *enemies2 = Level_getEnemies(pp(this, 0x0));
    AEGeometry_addChild(*(void **)(*(char **)((char *)enemies2 + 4) + 8), u32(pp(this, 0x64), 0xc));

    if (positions == 0)
        return;

    char *data = *(char **)((char *)positions + 4);
    for (unsigned int i = 0; i < *(unsigned int *)positions; i++) {
        void *slot = *(void **)(data + i * 4);
        if (slot != 0) {
            ArrayReleaseClasses_VectorPtr(slot);
            void *s2 = *(void **)(data + i * 4);
            if (s2 != 0)
                operator_delete(Array_VectorPtr_dtor(s2));
            *(void **)(data + i * 4) = 0;
        }
    }
    ArrayReleaseClasses_ArrayVectorPtr(positions);
    Array_ArrayVectorPtr_dtor(positions);
    CutScene_turretFinalize();
}
