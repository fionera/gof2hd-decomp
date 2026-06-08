#include "class.h"

// RepairBeam::~RepairBeam() — releases the contained AEGeometry* array (also releasing the
// pointed-to objects) and the int/float arrays, nulling each handle.
extern "C" void RepairBeam_ArrayReleaseClasses_Geo(void *arr);
extern "C" void *RepairBeam_Array_Geo_dtor(void *arr);
extern "C" void *RepairBeam_Array_int_dtor(void *arr);
extern "C" void *RepairBeam_Array_float_dtor(void *arr);
extern "C" void operator_delete(void *p);

extern "C" RepairBeam *RepairBeam_dtor(RepairBeam *self)
{
    void *geoms = F<void *>(self, 0x10);
    if (geoms != 0) {
        RepairBeam_ArrayReleaseClasses_Geo(geoms);
        void *g = F<void *>(self, 0x10);
        if (g != 0) {
            operator_delete(RepairBeam_Array_Geo_dtor(g));
        }
        F<void *>(self, 0x10) = 0;
    }
    void *ids = F<void *>(self, 0x14);
    if (ids != 0) {
        operator_delete(RepairBeam_Array_int_dtor(ids));
    }
    F<void *>(self, 0x14) = 0;
    void *charges = F<void *>(self, 0x18);
    if (charges != 0) {
        operator_delete(RepairBeam_Array_float_dtor(charges));
    }
    F<void *>(self, 0x18) = 0;
    return self;
}
