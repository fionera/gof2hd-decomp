#include "class.h"

struct AEGeometry;

namespace AbyssEngine {
namespace AERandom { int nextInt(int rng); }                  // 0x71fc8
namespace PaintCanvas { unsigned TransformGetTransform(unsigned canvas); } // 0x72088
namespace Transform { void SetAnimationState(unsigned tf, int a, int b); } // 0x6fd18
}

extern "C" int Item_getAttribute(int item, int attr);         // 0x718e4
extern "C" void *Gun_operator_new(unsigned size);             // operator new(uint)
extern "C" void *Gun_operator_new_arr(unsigned size);         // operator new[](uint)
extern "C" __attribute__((nothrow)) void Gun_operator_delete(void *p); // operator delete(void*)
// AEGeometry::AEGeometry(AEGeometry*, unsigned short, PaintCanvas*, bool)
extern "C" void AEGeometry_ctor(AEGeometry *self, unsigned short idx, void *canvas, bool b);
extern "C" void *AEGeometry_dtor(AEGeometry *self);           // AEGeometry::~AEGeometry

// pc-rel globals (each a holder; *holder used).
extern int *const gSI_items   __attribute__((visibility("hidden")));
extern int  *const gSI_table  __attribute__((visibility("hidden")));
extern unsigned *const gSI_canvas __attribute__((visibility("hidden")));
extern int  *const gSI_rng    __attribute__((visibility("hidden")));

extern "C" void Gun_setIndex(Gun *self, int index)
{
    self->f_58 = index;
    int *items = gSI_items;
    self->f_108 = (index == 0xe4) || ((unsigned)(index - 9) < 3);
    self->f_64 = Item_getAttribute(*(int *)(*(int *)(*items + 4) + index * 4), 0xa);
    int g = gSI_table[index];
    if (g >= 0) {
        unsigned count = self->f_8;
        unsigned long long bytes = (unsigned long long)count * 4;
        unsigned alloc = (unsigned)bytes;
        if ((unsigned)(bytes >> 32) != 0)
            alloc = 0xffffffff;
        self->f_10c = Gun_operator_new_arr(alloc);
        self->f_110 = Gun_operator_new_arr(count);
        unsigned *canvasHolder = gSI_canvas;
        int *rngHolder = gSI_rng;
        for (unsigned i = 0; i < count; i = i + 1) {
            AEGeometry *geom = (AEGeometry *)Gun_operator_new(0xc0);
            AEGeometry_ctor(geom, (unsigned short)g, (void *)*canvasHolder, false);
            ((int *)self->f_10c)[i] = *(int *)((char *)geom + 0xc);
            int r = AbyssEngine::AERandom::nextInt(*rngHolder);
            ((uint8_t *)self->f_110)[i] = (r == 0);
            unsigned tf = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder);
            AbyssEngine::Transform::SetAnimationState(tf, 0, 0);
            void *p = AEGeometry_dtor(geom);
            Gun_operator_delete(p);
            count = self->f_8;
        }
    }
}
