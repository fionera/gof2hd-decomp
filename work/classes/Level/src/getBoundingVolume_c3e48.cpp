#include "class.h"

struct FileRead;
struct BoundingVolume;

__attribute__((visibility("hidden"))) extern int *g_gbv_stack; // [DAT_000d405c]

extern "C" {
void *Level_opnew_gbv(unsigned size);
void  FileRead_ctor_gbv(FileRead *fr);
void *FileRead_dtor_gbv(FileRead *fr);
int   FileRead_loadStationCollision_gbv(int fr);
int   FileRead_loadStaticCollision_gbv(int fr);
void  operator_delete_gbv(void *p);
void  ArrayBV_ctor_gbv(void *a);
void *ArrayInt_dtor_gbv(void *a);
void  ArraySetLength_BV_gbv(unsigned n, void *a);
void  ArrayRelease_int_gbv(void *a);
// Builds one BoundingVolume (sphere when shape==0, AAB when shape==1) from the raw collision
// record at `rec`; returns the new object. The conversion is fixed-point->float SIMD math the
// decompiler mangled, so it lives in the engine helper.
BoundingVolume *Level_gbv_makeVolume(int rec, int shape);
}

// Level::getBoundingVolume(int idx, AEGeometry* outArrayHolder) — loads the per-mesh collision
// data for a station (idx<2000) or static object and produces an Array<BoundingVolume*>.
extern "C" void *Level_getBoundingVolume(int idx, int kind)
{
    (void)idx;
    FileRead *fr = (FileRead *)Level_opnew_gbv(1);
    FileRead_ctor_gbv(fr);

    int *coll;
    if (kind < 2000)
        coll = (int *)FileRead_loadStationCollision_gbv((int)fr);
    else
        coll = (int *)FileRead_loadStaticCollision_gbv((int)fr);
    operator_delete_gbv(FileRead_dtor_gbv(fr));

    void *result = 0;
    if (coll != 0) {
        unsigned n = *(unsigned *)coll[1];   // entry count
        void *arr = Level_opnew_gbv(0xc);
        ArrayBV_ctor_gbv(arr);
        ArraySetLength_BV_gbv(n, arr);
        result = arr;

        int cursor = 1;
        for (unsigned i = 0; i < n; i = i + 1) {
            int *data = (int *)coll[1];
            int shape = data[cursor];
            int rec = (int)(data + cursor);
            BoundingVolume *bv = 0;
            if (shape == 1) {
                bv = Level_gbv_makeVolume(rec, 1);
                cursor = cursor + 7;
            } else if (shape == 0) {
                bv = Level_gbv_makeVolume(rec, 0);
                cursor = cursor + 5;
            } else {
                cursor = cursor + 1;
            }
            ((BoundingVolume **)(*(int *)((char *)arr + 4)))[i] = bv;
        }

        ArrayRelease_int_gbv(coll);
        operator_delete_gbv(ArrayInt_dtor_gbv(coll));
    }
    return result;
}
