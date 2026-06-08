#include "class.h"

extern "C" {
void *Level_getEnemies(void *level);
void PaintCanvas_TransformRemoveChild(void *canvas, unsigned int parent, unsigned int child);
void *AEGeometry_getMatrix(void *geom);
void __aeabi_memcpy(void *dst, const void *src, unsigned int n);
void *Globals_getShipGroup(void *globals, int type, int slot, bool flag);
void *AEGeometry_setMatrix(void *matrix);
float VectorSignedToFloat(int v, int mode);
void PlayerFighter_setExhaustVisible(void *fighter, bool vis);
void LODManager_removeObject(void *lod, void *geom);
void *AEGeometry_dtor(void *geom);
void operator_delete(void *p);
}

__attribute__((visibility("hidden"))) extern void **g_canvas;
__attribute__((visibility("hidden"))) extern void **g_globals;
__attribute__((visibility("hidden"))) extern int CutScene_shipBankTable[];

void CutScene::replacePlayerShip(int a, int b)
{
    void *self = (void *)(long)a;

    if (Level_getEnemies(pp(self, 0x0)) == 0)
        return;

    void *enemies = Level_getEnemies(pp(self, 0x0));
    void *lead = *(void **)(*(char **)((char *)enemies + 4) + 8);
    void *oldGeom = lead;
    if (oldGeom != 0) {
        if (pp(self, 0x64) != 0) {
            void *canvas = *g_canvas;
            void *en2 = Level_getEnemies(pp(self, 0x0));
            unsigned int parent = u32(*(void **)(*(char **)((char *)en2 + 4) + 8), 0xc);
            unsigned int child = u32(pp(self, 0x64), 0xc);
            PaintCanvas_TransformRemoveChild(canvas, parent, child);
        }

        char matrix[0x3c];
        void *m = AEGeometry_getMatrix(oldGeom);
        __aeabi_memcpy(matrix, m, 0x3c);

        void *grp = Globals_getShipGroup(*g_globals, b, 0, false);

        void *en3 = Level_getEnemies(pp(self, 0x0));
        *(void **)(*(char **)((char *)en3 + 4) + 8) = grp;

        void *en4 = Level_getEnemies(pp(self, 0x0));
        AEGeometry_setMatrix(*(void **)(*(char **)((char *)en4 + 4) + 8));

        void *en5 = Level_getEnemies(pp(self, 0x0));
        void *ship = *(void **)((char *)en5 + 4);
        float bank = VectorSignedToFloat(CutScene_shipBankTable[b], 0);
        void *obj = *(void **)ship;
        typedef void (*BankFn)(void *, int, float, int);
        BankFn fn = *(BankFn *)(*(char **)obj + 0x48);
        fn(obj, 0, bank, 0);

        void *en6 = Level_getEnemies(pp(self, 0x0));
        PlayerFighter_setExhaustVisible(*(void **)((char *)en6 + 4), false);

        LODManager_removeObject(*(void **)self, oldGeom);
        operator_delete(AEGeometry_dtor(oldGeom));
    }

    checkForTurret();
}
