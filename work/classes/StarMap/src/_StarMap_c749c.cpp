#include "class.h"

extern "C" void ArrayReleaseClasses_Vector(void *arr);
extern "C" void *Array_Vector_dtor(void *arr);
extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" void *SystemPathFinder_dtor(void *finder);

StarMap::~StarMap()
{
    void *p;

    p = ptr_field(this, 0x194);
    if (p != 0) {
        ArrayReleaseClasses_Vector(p);
        p = ptr_field(this, 0x194);
        if (p != 0) {
            operator delete(Array_Vector_dtor(p));
        }
    }
    ptr_field(this, 0x194) = 0;

    p = ptr_field(this, 0x198);
    if (p != 0) {
        ArrayReleaseClasses_Vector(p);
        p = ptr_field(this, 0x198);
        if (p != 0) {
            operator delete(Array_Vector_dtor(p));
        }
    }
    ptr_field(this, 0x198) = 0;

    p = ptr_field(this, 0x1bc);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1bc) = 0;

    p = ptr_field(this, 0x1b0);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b0) = 0;

    p = ptr_field(this, 0x1b4);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b4) = 0;

    p = ptr_field(this, 0x1b8);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b8) = 0;

    p = ptr_field(this, 0x17c);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x17c) = 0;

    p = ptr_field(this, 0x180);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x180) = 0;

    p = ptr_field(this, 0x184);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x184) = 0;

    p = ptr_field(this, 0xf8);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0xf8) = 0;

    p = ptr_field(this, 0x50);
    if (p != 0) {
        operator delete(SystemPathFinder_dtor(p));
    }
    ptr_field(this, 0x50) = 0;
}
