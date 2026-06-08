#include "class.h"

// AbyssEngine::MeshReadData(Engine*, unsigned int const& handle, unsigned int flags,
//                           Mesh** slot, Material*)
// Read the binary vertex payload of a mesh from an open file. Driven by the per-mesh vertex
// format byte (mesh+0x00) and the file-format flags:
//   - bit at 0x1b -> indexed: read index count (mesh+0x28) and 16-bit index array (mesh+0x2c)
//   - positions (mesh+0x04): either 16-bit compressed (flags<<0x1d) or raw float
//   - while reading positions the axis-aligned bounding box is accumulated -> BSphere @0x3c/0x48
//   - bit 0x1e -> UVs (mesh+0x08): compressed shorts or raw floats (V optionally flipped)
//   - bit 0x1d -> normals (mesh+0x10): decoded, normalized and clamped; tangents/binormals
//     (mesh+0x14/0x18) are then generated per-triangle when tangents are enabled
//   - bit 0x1c -> colors (mesh+0x0c): unsigned bytes scaled to [0,1]
// Multi-submesh ("AEMes2") files recurse to read each child mesh. Returns 1, or -1 on error.
namespace AbyssEngine {

extern "C" {
void *AE_operator_new(size_t_ n);
void *AE_operator_new_arr(size_t_ n);
void  AE_operator_delete_arr(void *p);
int   AEFile_Read(unsigned int n, void *dst, unsigned int handle);
int   __aeabi_uidiv(int a, int b);
float AE_VectorSignedToFloat(int v, unsigned char mode);
float AE_VectorUnsignedToFloat(unsigned int v, unsigned char mode);
float AE_VectorLength(const void *v);
float AE_VectorDot(const void *a, const void *b);
void  AE_VectorNormalize(void *out, const void *in);
void  AE_VectorCross(void *a, const void *b);
void  AE_Vector_assign(void *dst, const void *src);
void  AE_VectorSub(void *self, const void *rhs);
void  AE_VectorMul(void *out, float s);
void  AE_BSphere_Merge(void *dst, const void *src);
void  AE_ArrayAdd_MeshPtr(void *item, void *arr);
int   AE_Mesh_ReadEnhancedDataFromFile(Mesh *self, unsigned int handle, unsigned int flags);
float sqrtf(float);

char *g_uvFlipFlag;        // **(DAT + 0x7c904) / 0x7c9d2
char *g_tangentEnabled;    // **(DAT + 0x7cc36)
}

int MeshReadData(Engine *engine, unsigned int *handlePtr, unsigned int flags, Mesh **slot,
                 Material *mat);

int MeshReadData(Engine *engine, unsigned int *handlePtr, unsigned int flags, Mesh **slot,
                 Material *mat)
{
    unsigned int handle = *handlePtr;
    unsigned int subBit = flags & 0x1a;
    unsigned char mode = 0;

    // Optional skinning/extra matrix block.
    if (subBit != 0) {
        if (AEFile_Read(0xc, (char *)*slot + 0x50, handle) == 0)
            return -1;
    }

    char *m = (char *)*slot;
    // Indexed geometry.
    if (u8(m, 0x0) & 0x10) { // (byte<<0x1b) negative
        if (AEFile_Read(2, m + 0x28, handle) == 0)
            return -1;
        void *idx = AE_operator_new_arr((unsigned int)u16(m, 0x28) << 1);
        pp((char *)*slot, 0x2c) = idx;
        if (AEFile_Read((unsigned int)u16((char *)*slot, 0x28) << 1, pp((char *)*slot, 0x2c),
                        handle) == 0)
            return -1;
        m = (char *)*slot;
    }

    // Vertex count.
    if (AEFile_Read(2, m + 0x2, handle) == 0)
        return -1;

    // Bounding-box accumulators (min/max for x,y,z).
    float minv[3] = {1e30f, 1e30f, 1e30f};
    float maxv[3] = {-1e30f, -1e30f, -1e30f};
    bool compressedPos = (int)(flags << 0x1d) < 0;
    unsigned int vcount = u16((char *)*slot, 0x2);

    if (compressedPos) {
        // 16-bit compressed positions: 3 shorts per vertex.
        void *raw = AE_operator_new_arr(vcount * 6);
        if (AEFile_Read(vcount * 6, raw, handle) == 0) { AE_operator_delete_arr(raw); return -1; }
        m = (char *)*slot;
        void *pos = AE_operator_new_arr(vcount * 0xc);
        pp(m, 0x4) = pos;
        m = (char *)*slot;
        int dst = 0;
        unsigned int n3 = vcount * 3;
        for (unsigned int i = 0; i < n3; ++i) {
            int axis = (int)__aeabi_uidiv((int)i, 3);
            axis = (int)i - axis * 3;
            float v = AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2), mode);
            *(float *)(*(int *)(m + 4) + dst) = v;
            dst += 4;
            if (v < minv[axis]) minv[axis] = v;
            if (maxv[axis] < v) maxv[axis] = v;
        }
        AE_operator_delete_arr(raw);
    } else if ((flags & 3) == 0) {
        // Raw float positions, read straight in (no per-element bbox here for the enhanced path).
        if ((flags & 0x18) != 0) {
            m = (char *)*slot;
            void *pos = AE_operator_new_arr(vcount * 0xc);
            pp(m, 0x4) = pos;
            if (AEFile_Read(u16((char *)*slot, 0x2) * 0xc, pp((char *)*slot, 0x4), handle) == 0)
                return -1;
        }
    } else {
        // Float positions, element-by-element with bbox.
        void *raw = AE_operator_new_arr(vcount * 0xc);
        if (AEFile_Read(vcount * 0xc, raw, handle) == 0) { AE_operator_delete_arr(raw); return -1; }
        m = (char *)*slot;
        void *pos = AE_operator_new_arr(vcount * 0xc);
        pp(m, 0x4) = pos;
        m = (char *)*slot;
        int dst = 0;
        unsigned int n3 = vcount * 3;
        for (unsigned int i = 0; i < n3; ++i) {
            int axis = (int)__aeabi_uidiv((int)i, 3);
            axis = (int)i - axis * 3;
            float v = AE_VectorSignedToFloat(*(int *)((char *)raw + i * 4), mode);
            *(float *)(*(int *)(m + 4) + dst) = v;
            dst += 4;
            if (v < minv[axis]) minv[axis] = v;
            if (maxv[axis] < v) maxv[axis] = v;
        }
        AE_operator_delete_arr(raw);
    }

    // BSphere center = box center, radius = half-diagonal length.
    float center[3];
    center[0] = (maxv[0] + minv[0]) * 0.5f;
    center[1] = (maxv[1] + minv[1]) * 0.5f;
    center[2] = (maxv[2] + minv[2]) * 0.5f;
    AE_Vector_assign((char *)*slot + 0x3c, center);
    float halfDiag[3] = {minv[0], minv[1], minv[2]};
    AE_VectorSub(center, halfDiag);
    f32((char *)*slot, 0x48) = AE_VectorLength(center);

    m = (char *)*slot;
    // UV coordinates.
    if (u8(m, 0x0) & 4) { // (byte<<0x1e) negative
        if (compressedPos) {
            void *raw = AE_operator_new_arr(vcount << 2);
            if (AEFile_Read(vcount << 2, raw, handle) == 0) { AE_operator_delete_arr(raw); return -1; }
            m = (char *)*slot;
            void *uv = AE_operator_new_arr(vcount << 3);
            pp(m, 0x8) = uv;
            m = (char *)*slot;
            char flip = *g_uvFlipFlag;
            int dst = 0;
            const double scale = 1.0 / 32767.0;
            for (unsigned int i = 0; i < (vcount << 1); i += 2) {
                double u = (double)AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2), mode) * scale;
                float *p = (float *)(*(int *)(m + 8) + dst);
                dst += 8;
                p[0] = (float)u;
                double v = (double)AE_VectorSignedToFloat((int)*(short *)((char *)raw + i * 2 + 2), mode) * scale;
                double vv = (flip == 0) ? v : (1.0 - v);
                p[1] = (float)vv;
            }
            AE_operator_delete_arr(raw);
        } else if ((flags & 0x18) != 0) {
            void *uv = AE_operator_new_arr(vcount << 3);
            pp(m, 0x8) = uv;
            if (AEFile_Read(u16((char *)*slot, 0x2) << 3, pp((char *)*slot, 0x8), handle) == 0)
                return -1;
            if (*g_uvFlipFlag != 0) {
                m = (char *)*slot;
                int off = 4;
                for (unsigned int i = 0; i < (unsigned int)(u16(m, 0x2) << 1); i += 2) {
                    float *p = (float *)(*(int *)(m + 8) + off);
                    off += 8;
                    *p = 1.0f - *p;
                }
            }
        }
    }

    m = (char *)*slot;
    // Normals (+ tangent/binormal generation).
    if (u8(m, 0x0) & 8) { // (byte<<0x1d) negative
        if (compressedPos) {
            void *raw = AE_operator_new_arr(vcount * 6);
            if (AEFile_Read(vcount * 6, raw, handle) == 0) { AE_operator_delete_arr(raw); return -1; }
            m = (char *)*slot;
            void *nrm = AE_operator_new_arr(vcount * 0xc);
            pp(m, 0x10) = nrm;
            m = (char *)*slot;
            const double scale = 1.0 / 32767.0;
            short *s = (short *)raw;
            int off = 4;
            for (unsigned int i = 0; i < vcount * 3; i += 3) {
                float nx = (float)((double)AE_VectorSignedToFloat((int)s[0], mode) * scale);
                float ny = (float)((double)AE_VectorSignedToFloat((int)s[1], mode) * scale);
                float nz = (float)((double)AE_VectorSignedToFloat((int)s[2], mode) * scale);
                float len2 = nx * nx + ny * ny + nz * nz;
                float len = sqrtf(len2);
                int base = *(int *)(m + 0x10);
                if (len != 0.0f) {
                    nx /= len; ny /= len; nz /= len;
                    if (nx < -1.0f) nx = -1.0f; if (nx > 1.0f) nx = 1.0f;
                    if (ny < -1.0f) ny = -1.0f; if (ny > 1.0f) ny = 1.0f;
                    if (nz < -1.0f) nz = -1.0f; if (nz > 1.0f) nz = 1.0f;
                    ((float *)(base + off))[-1] = ny;
                    *(float *)(base + off) = nx;
                    *(float *)(base + off + 4) = nz;
                } else {
                    *(unsigned int *)(base + off) = 0x3f800000;
                    *(unsigned int *)(base + off - 4) = 0;
                    *(float *)(base + off + 4) = 1.0f;
                }
                s += 3;
                off += 0xc;
                m = (char *)*slot;
            }
            AE_operator_delete_arr(raw);
        } else if ((flags & 0x18) != 0) {
            void *nrm = AE_operator_new_arr(vcount * 0xc);
            pp(m, 0x10) = nrm;
            if (AEFile_Read(u16((char *)*slot, 0x2) * 0xc, pp((char *)*slot, 0x10), handle) == 0)
                return -1;
        }

        // Per-triangle tangent/binormal generation.
        if (*g_tangentEnabled != 0) {
            m = (char *)*slot;
            void *tan = AE_operator_new_arr(vcount * 0xc);
            pp(m, 0x14) = tan;
            m = (char *)*slot;
            void *bin = AE_operator_new_arr(vcount * 0xc);
            pp(m, 0x18) = bin;
            m = (char *)*slot;

            unsigned int triCount = (unsigned int)__aeabi_uidiv((int)(unsigned short)u16(m, 0x28), 3);
            void *accum = AE_operator_new_arr(vcount * 0xc);
            // Zero accumulator.
            for (unsigned int b = 0; b < vcount * 0xc; ++b) ((char *)accum)[b] = 0;

            int triOff = 0;
            for (unsigned int t = 0; t < triCount; ++t) {
                m = (char *)*slot;
                int posBase = *(int *)(m + 4);
                int uvBase = *(int *)(m + 8);
                int idxBase = *(int *)(m + 0x2c) + triOff;
                unsigned int i0 = (unsigned int)*(unsigned short *)(idxBase);
                unsigned int i2 = (unsigned int)*(unsigned short *)(idxBase + 4);
                unsigned int i1 = (unsigned int)*(unsigned short *)(idxBase + 2);

                float uv0v = *(float *)((i0 << 3 | 4) + uvBase);
                float uv0u = *(float *)(uvBase + i0 * 8);
                float dv1 = *(float *)((i2 << 3 | 4) + uvBase) - uv0v;
                float dv2 = *(float *)((i1 << 3 | 4) + uvBase) - uv0v;
                float *p0 = (float *)(posBase + i0 * 0xc);
                float *p2 = (float *)(posBase + i2 * 0xc);
                float *p1 = (float *)(posBase + i1 * 0xc);
                float denom = (*(float *)(uvBase + i1 * 8) - uv0u) * dv1 -
                              (*(float *)(uvBase + i2 * 8) - uv0u) * dv2;
                float r = (denom != 0.0f) ? (1.0f / denom) : 0.0f;
                float tng[3];
                tng[1] = ((p1[1] - p0[1]) * dv1 - (p2[1] - p0[1]) * dv2) * r;
                tng[0] = ((p1[0] - p0[0]) * dv1 - (p2[0] - p0[0]) * dv2) * r;
                tng[2] = ((p1[2] - p0[2]) * dv1 - (p2[2] - p0[2]) * dv2) * r;
                float *a = (float *)((int)accum + i0 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                a = (float *)((int)accum + i1 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                a = (float *)((int)accum + i2 * 0xc);
                a[0] += tng[0]; a[1] += tng[1]; a[2] += tng[2];
                triOff += 6;
            }

            // Orthonormalize tangent against the normal, store tangent/binormal.
            int off = 0;
            for (unsigned int v = 0; v < vcount; ++v) {
                m = (char *)*slot;
                float nrm[3];
                nrm[0] = *(float *)(*(int *)(m + 0x10) + off);
                nrm[1] = *(float *)(*(int *)(m + 0x10) + off + 4);
                nrm[2] = *(float *)(*(int *)(m + 0x10) + off + 8);
                float tg[3];
                tg[0] = *(float *)((int)accum + off);
                tg[1] = *(float *)((int)accum + off + 4);
                tg[2] = *(float *)((int)accum + off + 8);
                float d = AE_VectorDot(nrm, tg);
                float scaled[3] = {nrm[0], nrm[1], nrm[2]};
                AE_VectorMul(scaled, d);
                AE_VectorSub(tg, scaled);
                float tanOut[3];
                AE_VectorNormalize(tanOut, tg);
                int tb = *(int *)((char *)*slot + 0x14) + off;
                *(float *)(tb) = tanOut[0];
                *(float *)(tb + 4) = tanOut[1];
                *(float *)(tb + 8) = tanOut[2];
                float binOut[3] = {tg[0], tg[1], tg[2]};
                AE_VectorCross(binOut, nrm);
                int bb = *(int *)((char *)*slot + 0x18) + off;
                *(float *)(bb) = binOut[0];
                *(float *)(bb + 4) = binOut[1];
                *(float *)(bb + 8) = binOut[2];
                off += 0xc;
            }
            AE_operator_delete_arr(accum);
        }
    }

    m = (char *)*slot;
    // Vertex colors. (byte<<0x1c) negative -> bit 0x10 set.
    if (u8(m, 0x0) & 0x10) {
        if (compressedPos) {
            void *raw = AE_operator_new_arr(vcount << 2);
            if (AEFile_Read(vcount << 2, raw, handle) == 0) { AE_operator_delete_arr(raw); return -1; }
            m = (char *)*slot;
            void *col = AE_operator_new_arr(vcount << 4);
            pp(m, 0xc) = col;
            m = (char *)*slot;
            const float inv = 255.0f;
            int dst = 0;
            for (unsigned int i = 0; i < (vcount << 2); ++i) {
                float c = AE_VectorUnsignedToFloat((unsigned int)*((unsigned char *)raw + i), mode);
                *(float *)(*(int *)(m + 0xc) + dst) = c / inv;
                dst += 4;
            }
            AE_operator_delete_arr(raw);
        } else if ((flags & 0x18) != 0) {
            void *col = AE_operator_new_arr(vcount << 4);
            pp(m, 0xc) = col;
            if (AEFile_Read(u16((char *)*slot, 0x2) << 4, pp((char *)*slot, 0xc), handle) == 0)
                return -1;
        }
    }

    // Enhanced data / submesh recursion.
    if (subBit != 0) {
        if (AE_Mesh_ReadEnhancedDataFromFile(*slot, handle, flags) == 0)
            return -1;
        unsigned short childCount = 0;
        if (AEFile_Read(2, &childCount, handle) == 0)
            return -1;
        int xf = *(int *)((char *)*slot + 0x34);
        if (xf != 0)
            AE_BSphere_Merge((char *)*slot + 0x3c, (char *)(xf + 0xd4));
        for (unsigned int c = 0; c < childCount; ++c) {
            char *child = (char *)AE_operator_new(0x88);
            for (int b = 0; b < 0x88; ++b) child[b] = 0;
            u32(child, 0x4c) = 0x3f800000;
            u8(child, 0x84) = 1;
            u8(child, 0x0) = u8((char *)*slot, 0x0);
            pp(child, 0x30) = pp((char *)*slot, 0x30);
            Mesh *childPtr = (Mesh *)child;
            if (MeshReadData(engine, handlePtr, flags, &childPtr, mat) == -1)
                return -1;
            AE_BSphere_Merge((char *)*slot + 0x3c, (char *)childPtr + 0x3c);
            AE_ArrayAdd_MeshPtr(childPtr, (char *)*(int *)((char *)*slot + 0x34) + 0x3c);
        }
    }

    return 1;
}

} // namespace AbyssEngine
