#include "class.h"

extern "C" void AEGeometry_setPosition(void *geom, const Vector &v);
extern "C" void *AEGeometry_getMatrix(void *geom);
extern "C" void *Matrix_assign(void *dst, void *src);
extern "C" void *Matrix_mulassign(void *dst, void *src);

extern "C" void KIPlayer_setPosition_vec(KIPlayer *self, const Vector &v)
{
    void *geom = *(void **)((char *)self + 0x8);
    if (geom != 0) {
        AEGeometry_setPosition(geom, v);
        void *m = AEGeometry_getMatrix(*(void **)((char *)self + 0x8));
        Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, m);
        void *cond = *(void **)((char *)self + 0xc);
        if (cond != 0) {
            void *m2 = AEGeometry_getMatrix(cond);
            Matrix_mulassign(*(char **)((char *)self + 0x4) + 0x4, m2);
        }
    }
}
