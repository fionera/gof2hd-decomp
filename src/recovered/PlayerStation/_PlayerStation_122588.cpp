#include "class.h"

extern "C" void *PlayerStation_vtable __attribute__((visibility("hidden")));
extern "C" void *AEGeometry_destructor(void *geometry) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_BoundingVolumePtr(void *array) __attribute__((nothrow));
extern "C" void *Array_BoundingVolumePtr_destructor(void *array) __attribute__((nothrow));
PlayerStation::~PlayerStation() noexcept(false)
{
    P(this, 0x0) = (char *)PlayerStation_vtable + 8;

    void *geometry = P(this, 0x140);
    if (geometry != 0) {
        operator_delete(AEGeometry_destructor(geometry));
    }
    P(this, 0x140) = 0;

    geometry = P(this, 0x14c);
    if (geometry != 0) {
        operator_delete(AEGeometry_destructor(geometry));
    }
    P(this, 0x14c) = 0;

    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        ArrayReleaseClasses_BoundingVolumePtr(volumes);
        volumes = P(this, 0x130);
        if (volumes != 0) {
            operator_delete(Array_BoundingVolumePtr_destructor(volumes));
        }
    }
    P(this, 0x130) = 0;

}
