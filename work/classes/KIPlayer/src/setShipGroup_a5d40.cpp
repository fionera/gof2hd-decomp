#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void *gCanvas;
extern "C" void AEGeometry_ctor2(void *self, void *canvas);
extern "C" void AEGeometry_addChild(void *self, int child);
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void AEGeometry_setPosition4(void *geom, int a, int b, int c);
extern "C" void *AEGeometry_getMatrix(void);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void Matrix_mulassign(void *dst, void *src);

extern "C" void KIPlayer_setShipGroup(KIPlayer *self, int param2, int flag, int cond)
{
    *(int *)((char *)self + 0x7c) = flag;
    if (param2 == 0 || cond == 0) {
        *(int *)((char *)self + 0x8) = param2;
        void *g = *(void **)((char *)self + 0xc);
        if (g != 0) operator_delete(AEGeometry_dtor(g));
        *(void **)((char *)self + 0xc) = 0;
    } else {
        void *grp = *(void **)((char *)self + 0x8);
        *(int *)((char *)self + 0xc) = param2;
        if (grp == 0) {
            grp = operator_new(0xc0);
            AEGeometry_ctor2(grp, gCanvas);
            *(void **)((char *)self + 0x8) = grp;
        }
        AEGeometry_addChild(grp, *(int *)(*(char **)((char *)self + 0xc) + 0xc));
        *(int *)(*(char **)((char *)self + 0xc) + 0x24) = *(int *)(*(char **)((char *)self + 0x8) + 0xc);
    }
    AEGeometry_setPosition4(*(void **)((char *)self + 0x8),
                            *(int *)((char *)self + 0x58),
                            *(int *)((char *)self + 0x5c),
                            *(int *)((char *)self + 0x60));
    Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    if (*(int *)((char *)self + 0xc) != 0) {
        Matrix_mulassign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    }
}
