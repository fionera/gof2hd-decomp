#include "class.h"

// AbyssEngine::MeshCreateFromFile(Engine*, char const*, Mesh**, Material*)
// Allocate and identity-initialize a Mesh (0x88 bytes), open the model file, classify its
// 7-byte magic into one of five format flags, read the submesh count, and dispatch to
// MeshReadData. Single-submesh files are read directly; multi-submesh files allocate a
// Transform (mesh+0x34) and read each child mesh into it, merging bounding spheres. After
// loading, animation data is collected. Returns 1 on success, -1 on error, -4 on bad args.
namespace AbyssEngine {

extern "C" {
void *AE_operator_new(size_t_ n);
void  AE_operator_delete(void *p);
int   AEFile_OpenRead(const char *path, void *handleOut, void *unused);
int   AEFile_Read(unsigned int n, void *dst, unsigned int handle);
void  AEFile_Close(unsigned int handle);
void  AE_Transform_ctor(void *self);
void  AE_Transform_CollectAnimationData(void *self);
void  AE_Transform_SetAnimationRangeInTime(void *self, long long t);
void  AE_BSphere_Merge(void *dst, const void *src);
void  AE_ArrayAdd_MeshPtr(void *item, void *arr);
long long __aeabi_f2lz(float v);
}

int  MeshReadData(Engine *engine, unsigned int *handle, unsigned int flags, Mesh **slot,
                  Material *mat);
void MeshRelease(Engine *engine, Mesh **slot);

// Zero a freshly-allocated Mesh and write its identity defaults.
static void initMesh(char *m)
{
    for (int i = 0; i < 0x88; ++i) m[i] = 0;
    // Identity-ish: scale field at 0x4c = 1.0; the matrix slots default to 0 here.
    u32(m, 0x4c) = 0x3f800000;
}

int MeshCreateFromFile(Engine *engine, char *path, Mesh **out, Material *mat)
{
    if (engine == 0 || path == 0)
        return -4;

    char *m = (char *)AE_operator_new(0x88);
    initMesh(m);
    *out = (Mesh *)m;
    pp(m, 0x30) = mat;

    unsigned int handle = 0;
    if (AEFile_OpenRead(path, &handle, (char *)m + 0x60) == 0) {
        if (*out != 0)
            AE_operator_delete((void *)*out);
        *out = 0;
        return -1;
    }

    char magic[7];
    for (int i = 0; i < 4; ++i) magic[i] = '*';
    if (AEFile_Read(7, magic, handle) == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    // Classify magic into a 5-bit format mask (one bit per known signature).
    static const char sig0[7] = {'A','E','M','e','s','h',0};
    static const char sig1[7] = {'A','E','A','n','i','m',0};
    static const char sig2[7] = {'A','E','S','k','i','n',0};
    static const char sig3[7] = {'A','E','L','o','d','0',0};
    static const char sig4[7] = {'A','E','M','e','s','2',0};
    unsigned int fmt = 0x1f;
    for (int i = 0; i < 7; ++i) {
        char ch = magic[i];
        if (sig0[i] != ch) fmt &= ~1u;
        if (sig1[i] != ch) fmt &= ~8u;
        if (sig2[i] != ch) fmt &= ~0x10u;
        if (sig3[i] != ch) fmt &= ~4u;   // sig3 -> bit2 (LOD)
        if (sig4[i] != ch) fmt &= ~2u;
    }

    if (fmt == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    // Optional version word for some formats.
    if ((fmt & 0x1b) != 0) {
        unsigned short ver = 0;
        if (AEFile_Read(2, &ver, handle) == 0) { MeshRelease(engine, out); AEFile_Close(handle); return -1; }
    }
    // Read the first byte (vertex-format flag) into mesh+0x00.
    if (AEFile_Read(1, (char *)*out, handle) == 0 || u8((char *)*out, 0x0) == 0) {
        MeshRelease(engine, out);
        AEFile_Close(handle);
        return -1;
    }

    bool ok = false;
    if ((fmt & 0x1a) == 0) {
        if (MeshReadData(engine, &handle, fmt, out, mat) != -1)
            ok = true;
    } else {
        unsigned short subCount = 0;
        if (AEFile_Read(2, &subCount, handle) == 0) {
            MeshRelease(engine, out);
            AEFile_Close(handle);
            return -1;
        }
        if (subCount < 2) {
            if (MeshReadData(engine, &handle, fmt, out, mat) != -1)
                ok = true;
        } else {
            char *xf = (char *)AE_operator_new(0x180);
            AE_Transform_ctor(xf);
            pp((char *)*out + 0x34, 0) = xf;
            for (unsigned int s = 0; s < subCount; ++s) {
                char *child = (char *)AE_operator_new(0x88);
                initMesh(child);
                u8(child, 0x84) = 1;
                u8(child, 0x0) = u8((char *)*out, 0x0);
                pp(child, 0x30) = mat;
                Mesh *childPtr = (Mesh *)child;
                if (MeshReadData(engine, &handle, fmt, &childPtr, mat) == -1) {
                    MeshRelease(engine, out);
                    AEFile_Close(handle);
                    return -1;
                }
                AE_BSphere_Merge((char *)*out + 0x3c, (char *)childPtr + 0x3c);
                AE_ArrayAdd_MeshPtr(childPtr, (char *)*(int *)((char *)*out + 0x34) + 0x3c);
            }
            ok = true;
        }
    }

    if (ok) {
        AEFile_Close(handle);
        void *xf = pp((char *)*out + 0x34, 0);
        if (xf != 0) {
            AE_Transform_CollectAnimationData(xf);
            long long t = __aeabi_f2lz(0.0f);
            AE_Transform_SetAnimationRangeInTime(pp((char *)*out + 0x34, 0), t);
        }
        return 1;
    }

    MeshRelease(engine, out);
    AEFile_Close(handle);
    return -1;
}

} // namespace AbyssEngine
