#include "class.h"

struct RocketGun {
    RocketGun(int param_1, Gun *param_2, int param_3, int param_4,
              uint32_t param_5, int param_6, bool param_7, Level *param_8);
};

extern "C" __attribute__((visibility("hidden"))) void *RocketGun_vtable;
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder;
extern "C" void *operator_new(uint32_t size);
extern "C" void ObjectGun_ctor(RocketGun *self, int param_1, Gun *param_2, int param_3,
                                uint32_t param_5, Level *param_8);
extern "C" void AEGeometry_ctor(void *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void PaintCanvas_TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" void *AEGeometry_dtor(void *self);
extern "C" void operator_delete(void *p);

static const uint32_t kRocketC8 = 0x3dcccccd;

RocketGun::RocketGun(int param_1, Gun *param_2, int param_3, int param_4,
                     uint32_t param_5, int param_6, bool param_7, Level *param_8)
{
    ObjectGun_ctor(this, param_1, param_2, param_3, param_5, param_8);

    void *vtable = RocketGun_vtable;
    char *zeroBase = (char *)this + 0xd4;
    uint32_t c8 = kRocketC8;
    *(v4i *)zeroBase = (v4i){0, 0, 0, 0};
    F<int>(this, 0xb4) = 0;
    F<int>(this, 0xb8) = 0;
    F<int>(this, 0xbc) = 0;
    F<uint8_t>(this, 0xc0) = param_7;
    F<int>(this, 0xc4) = -1;
    F<uint32_t>(this, 0xc8) = c8;
    F<int>(this, 0xcc) = -1;
    F<int>(this, 0xd0) = param_6;
    F<int>(this, 0xe4) = 0;
    *(void **)this = (char *)vtable + 8;
    F<int>(this, 0x4) = -1;

    int gate = 0x37a9;
    if (param_3 != 0x37a9)
        gate = 0x37a7;
    if (param_3 == 0x37a9 || param_3 == gate) {
        void *geom = operator_new(0xc0);
        uint16_t mesh = 0x37aa;
        void **holder = RocketGun_canvas_holder;
        if (param_1 == 0x37a7)
            mesh = 0x37a8;
        AEGeometry_ctor(geom, mesh, *holder, false);
        PaintCanvas_TransformAddChild(*holder, F<uint32_t>(this, 0x10),
                                      F<uint32_t>(geom, 0xc));
        operator_delete(AEGeometry_dtor(geom));
    }
}
