#include "class.h"

extern "C" char KIPlayer_vtable;       // vtable symbol base
extern "C" void *Player_dtor(void *p);
extern "C" void *Route_dtor(Route *p);
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void *ArrayUint_dtor(void *p);
extern "C" void *ArrayInt_dtor(void *p);
extern "C" void ArrayReleaseClasses_SpacePoint(void *arr);
extern "C" void *ArraySpacePoint_dtor(void *p);
extern "C" void String_dtor(void *p);
extern "C" void operator_delete(void *p);

// Complete object destructor (D1). Returns this.
extern "C" void *_ZN8KIPlayerD1Ev(KIPlayer *self)
{
    *(void **)self = &KIPlayer_vtable + 8;

    void *p = *(void **)((char *)self + 0x4);
    if (p != 0) operator_delete(Player_dtor(p));
    *(void **)((char *)self + 0x4) = 0;

    Route *r = *(Route **)((char *)self + 0x6c);
    if (r != 0) operator_delete(Route_dtor(r));
    *(void **)((char *)self + 0x6c) = 0;

    void *g1 = *(void **)((char *)self + 0x78);
    if (g1 != 0) operator_delete(AEGeometry_dtor(g1));
    *(void **)((char *)self + 0x78) = 0;

    void *au = *(void **)((char *)self + 0xc4);
    if (au != 0) operator_delete(ArrayUint_dtor(au));
    *(void **)((char *)self + 0xc4) = 0;

    void *g2 = *(void **)((char *)self + 0x8);
    if (g2 != 0) operator_delete(AEGeometry_dtor(g2));
    *(void **)((char *)self + 0x8) = 0;

    void *g3 = *(void **)((char *)self + 0xc);
    if (g3 != 0) operator_delete(AEGeometry_dtor(g3));
    *(void **)((char *)self + 0xc) = 0;

    void *ai = *(void **)((char *)self + 0x50);
    if (ai != 0) operator_delete(ArrayInt_dtor(ai));
    *(void **)((char *)self + 0x50) = 0;

    void *sp = *(void **)((char *)self + 0xcc);
    if (sp != 0) {
        ArrayReleaseClasses_SpacePoint(sp);
        void *sp2 = *(void **)((char *)self + 0xcc);
        if (sp2 != 0) operator_delete(ArraySpacePoint_dtor(sp2));
        *(void **)((char *)self + 0xcc) = 0;
    }

    String_dtor((char *)self + 0x18);
    return self;
}
