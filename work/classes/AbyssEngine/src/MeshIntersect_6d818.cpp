#include "class.h"

extern "C" float sqrtf(float);

// AbyssEngine::MeshIntersect(float qx, float qz, AbyssEngine::Mesh* mesh)
// 2D pick test: walk the mesh's triangles (index buffer @0x2c, positions @0x4, uvs @0x8) and,
// for the triangle whose XZ bounding box contains the query point and which the point is
// "inside" (all three edge half-plane tests <= 0), return the averaged UV of that triangle.
// Returns the sentinel (DAT_0007dab8) packed into both lanes when nothing is hit. The result
// is a 2-float vector returned by value (sret in the original ABI).
namespace AbyssEngine {

struct Vec2 { float u, v; };

extern "C" float g_MeshIntersect_missValue; // DAT_0007dab8 sentinel
extern "C" char *g_MeshIntersect_flipVFlag; // *(DAT_0007dabc + 0x7da96)

Vec2 MeshIntersect(float qx, float qz, Mesh *mesh)
{
    Vec2 out;
    char *m = (char *)mesh;
    unsigned int i = 0;

    for (;;) {
        if ((unsigned int)(unsigned short)u16(m, 0x28) <= i) {
            out.u = g_MeshIntersect_missValue;
            out.v = g_MeshIntersect_missValue;
            return out;
        }

        float *pos = (float *)pp(m, 0x4);
        unsigned short *idx = (unsigned short *)((char *)pp(m, 0x2c) + i * 2);
        float *a = pos + (unsigned int)idx[0] * 3;
        float *b = pos + (unsigned int)idx[1] * 3;
        float *c = pos + (unsigned int)idx[2] * 3;

        float ax = a[0], az = a[2];
        float bx = b[0], bz = b[2];
        float cx = c[0], cz = c[2];

        // XZ bounding box of the triangle.
        float minZ = az, maxZ = bz;
        if (bz < az) { minZ = bz; maxZ = az; }
        float minX = ax, maxX = bx;
        if (bx < ax) { minX = bx; maxX = ax; }
        if (cz > maxZ) maxZ = cz;
        if (cz < minZ) minZ = cz;
        if (cx > maxX) maxX = cx;
        if (cx < minX) minX = cx;

        if (maxZ < qz && minX <= qx && maxX < qx && minZ <= qz) {
            // Edge tests using outward 2D normals; "inside" means all three <= 0.
            float ex = bx - ax, ez = bz - az;
            float len = sqrtf(ez * ez + ex * ex);
            float side = (ex / len) * qz + (-ez / len) * qx -
                         (az * (ex / len) + ax * (-ez / len));
            if (side <= 0.0f) {
                ex = cx - bx; ez = cz - bz;
                len = sqrtf(ez * ez + ex * ex);
                side = (ex / len) * qz + (-ez / len) * qx -
                       (bz * (ex / len) + bx * (-ez / len));
                if (side <= 0.0f) {
                    ez = az - cz; ex = ax - cx;
                    len = sqrtf(ez * ez + ex * ex);
                    side = (ex / len) * qz + (-ez / len) * qx -
                           (cz * (ex / len) + cx * (-ez / len));
                    if (side <= 0.0f) {
                        float *uv = (float *)pp(m, 0x8);
                        unsigned int i0 = idx[0], i1 = idx[1], i2 = idx[2];
                        out.u = (uv[i0 * 2] + uv[i1 * 2] + uv[i2 * 2]) / 3.0f;
                        float vv = (uv[i0 * 2 + 1] + uv[i1 * 2 + 1] + uv[i2 * 2 + 1]) / 3.0f;
                        out.v = vv;
                        if (*g_MeshIntersect_flipVFlag != 0)
                            out.v = 1.0f - vv;
                        return out;
                    }
                }
            }
        }
        i += 3;
    }
}

} // namespace AbyssEngine
