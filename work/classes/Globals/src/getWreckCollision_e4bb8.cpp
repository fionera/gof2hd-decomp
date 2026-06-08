#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_loadWreckCollision(int self);
extern "C" void *FileRead_dtor(void *self);
extern "C" void ArrayBV_ctor(void *a);
extern "C" void ArraySetLength_BV(unsigned n, void *a);
extern "C" void ArrayRelease_int(void *a);
extern "C" void *ArrayInt_dtor(void *a);
// Vector scale: in-place multiply of a Vector by a scalar, returns resulting magnitude/component.
extern "C" float VectorScale(void *vec, float scalar);
// BoundingSphere(center.x, center.y, center.z, radius)  (extra fp regs are scratch).
extern "C" void BoundingSphere_ctor(void *self, float cx, float cy, float cz, float r);
// BoundingAAB(maxX, maxY, maxZ, minX, minY, minZ) — 6 floats from the doubled extents.
extern "C" void BoundingAAB_ctor(void *self, float x0, float y0, float z0, float x1, float y1,
                                 float z1);

extern void *const gGWC_guardHolder __attribute__((visibility("hidden")));  // DAT_000f4e10

// Globals::getWreckCollision(int kind, AEGeometry* geom) — kind in r1.
extern "C" void Globals_getWreckCollision(void *retSlot, int kind, void *geom)
{
    (void)retSlot;
    int *guardP = *(int **)gGWC_guardHolder;
    volatile int saved = *guardP;

    void *fr = operator_new(1);
    FileRead_ctor(fr);
    int *data = (int *)FileRead_loadWreckCollision((int)(long)fr);
    operator_delete(FileRead_dtor(fr));

    void *outArr = 0;
    if (data != 0) {
        int count = **(int **)((char *)data + 4);

        float v[3] = {0, 0, 0};       // local_40..local_38 vector
        float c[3] = {0, 0, 0};       // local_34/2c/30 scalar parts

        outArr = operator_new(0xc);
        ArrayBV_ctor(outArr);
        ArraySetLength_BV((unsigned)count, outArr);

        int pos = 1;
        for (int i = 0; i < count; i++) {
            int *base = *(int **)((char *)data + 4);
            int kindWord = base[pos];
            void *bound = 0;
            if (kindWord == 1) {
                int *rec = base + pos;
                c[2] = (float)(-base[pos + 1]);
                c[0] = (float)rec[2];
                c[1] = (float)rec[3];
                v[0] = (float)rec[4];
                v[1] = (float)rec[5];
                v[2] = (float)rec[6];
                float mag = VectorScale(v, c[1]);
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = operator_new(0x2c);
                BoundingAAB_ctor(bound, v[2] + v[2], c[1] + c[1], v[1] + v[1], c[2], c[0], v[0]);
                pos += 7;
            } else if (kindWord == 0) {
                int *rec = base + pos;
                c[2] = (float)(-base[pos + 1]);
                c[0] = (float)rec[2];
                c[1] = (float)rec[3];
                v[0] = (float)rec[4];
                VectorScale(v, v[0]);
                float mag = v[0];
                if (v[0] < 0.0f) {
                    mag = VectorScale(v, v[0]);
                }
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = operator_new(0x48);
                BoundingSphere_ctor(bound, c[2], c[1], c[0], v[0]);
                pos += 5;
            } else {
                pos += 1;
                continue;
            }
            *(void **)(*(char **)((char *)outArr + 4) + i * 4) = bound;
        }

        ArrayRelease_int(data);
        operator_delete(ArrayInt_dtor(data));
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
