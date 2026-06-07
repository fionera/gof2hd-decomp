#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void **gCanvasPtr;
extern "C" void AEGeometry_ctor(void *self, unsigned short type, void *canvas, bool b);
extern "C" void AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void AEGeometry_setPosition_v(void *self, Vector *v);
extern "C" void *AEGeometry_getMatrix(void);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void Player_setKIPlayer(void *player, KIPlayer *self);

extern "C" void KIPlayer_createCrate(KIPlayer *self, int type)
{
    void *crate = operator_new(0xc0);
    unsigned short id;
    if (type == 1) {
        id = 0x421e;
    } else if (type == 2) {
        id = 0x421f;
    } else if (type == 3) {
        id = 0x4218;
    } else {
        int st = *(int *)((char *)self + 0x28);
        if (st == 1) {
            id = 0x425f;
        } else if (st == 3) {
            id = 0x425e;
        } else if (st == 9) {
            id = 0x4214;
        } else {
            id = 0x4261;
            if (st == 0) {
                id = 0x4260;
            }
        }
    }
    AEGeometry_ctor(crate, id, gCanvasPtr[0], false);
    *(void **)((char *)self + 0x78) = crate;
    Vector pos;
    AEGeometry_getPosition(&pos, *(void **)((char *)self + 0x8));
    AEGeometry_setPosition_v(crate, &pos);
    Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, AEGeometry_getMatrix());
    Player_setKIPlayer(*(void **)((char *)self + 0x4), self);
}
