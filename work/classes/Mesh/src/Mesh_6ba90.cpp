#include "class.h"

// AbyssEngine::Mesh::Mesh(Mesh*)  -- copy constructor.
// Resets the embedded BSphere at +0x3c, then (when given a source) flushes the source's
// VBO, copies the bounding sphere, the header/flags, the vertex pointers (the extra normal
// pair only when a global "has-normals" flag is set), deep-copies the animation Transform
// at +0x34, the embedded Vector at +0x50, and the remaining scalar fields.

namespace AbyssEngine {

namespace AEMath { struct BSphere; struct Vector; }

// Constant {0,0,1} tail the original loads into the fresh BSphere (radius defaults).
static const float kBSphereInit[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

// Global byte flag: when set, meshes carry the extra per-vertex normal data.
__attribute__((visibility("hidden"))) extern unsigned char *g_hasNormalsFlag;

extern "C" void _ZN11AbyssEngine4Mesh12ConvertToVBOEv(Mesh *m);
extern "C" AEMath::BSphere *_ZN11AbyssEngine6AEMath7BSphereaSERKS1_(
    AEMath::BSphere *self, const AEMath::BSphere *other);
extern "C" void *_ZN11AbyssEngine9TransformC1EPS0_(Transform *self, Transform *src);
extern "C" AEMath::Vector *_ZN11AbyssEngine6AEMath6VectoraSERKS1_(
    AEMath::Vector *self, const AEMath::Vector *other);

Mesh *Mesh_ctor(Mesh *self, Mesh *src)
{
    // Initialize the embedded BSphere at +0x3c.
    F<float>(self, 0x3c) = 0.0f;
    F<float>(self, 0x40) = kBSphereInit[1];
    F<float>(self, 0x44) = kBSphereInit[2];
    F<float>(self, 0x48) = kBSphereInit[3];
    F<float>(self, 0x4c) = 0.0f;
    F<float>(self, 0x50) = 0.0f;
    F<float>(self, 0x54) = 1.0f;

    if (src == 0)
        return self;

    if (F<unsigned char>(src, 0x84) != 0)
        _ZN11AbyssEngine4Mesh12ConvertToVBOEv(src);

    _ZN11AbyssEngine6AEMath7BSphereaSERKS1_(
        (AEMath::BSphere *)((char *)self + 0x3c),
        (const AEMath::BSphere *)((char *)src + 0x3c));

    unsigned char hasNormals = *g_hasNormalsFlag;

    F<uint32_t>(self, 0x7c) = 0;
    F<uint8_t>(self, 0x00) = F<uint8_t>(src, 0x00);
    F<uint32_t>(self, 0x1c) = 0;
    F<uint32_t>(self, 0x20) = 0;
    F<uint16_t>(self, 0x02) = F<uint16_t>(src, 0x02);
    F<uint32_t>(self, 0x04) = F<uint32_t>(src, 0x04);
    F<uint32_t>(self, 0x08) = F<uint32_t>(src, 0x08);
    F<uint32_t>(self, 0x0c) = F<uint32_t>(src, 0x0c);
    F<uint32_t>(self, 0x10) = F<uint32_t>(src, 0x10);
    if (hasNormals != 0) {
        F<uint32_t>(self, 0x14) = F<uint32_t>(src, 0x14);
        F<uint32_t>(self, 0x18) = F<uint32_t>(src, 0x18);
    }
    F<uint16_t>(self, 0x28) = F<uint16_t>(src, 0x28);
    F<uint16_t>(self, 0x2a) = F<uint16_t>(src, 0x2a);
    F<uint32_t>(self, 0x2c) = F<uint32_t>(src, 0x2c);
    F<uint32_t>(self, 0x30) = F<uint32_t>(src, 0x30);

    Transform *srcAnim = F<Transform *>(src, 0x34);
    if (srcAnim == 0) {
        F<Transform *>(self, 0x34) = 0;
    } else {
        Transform *anim = (Transform *)operator new(0x180);
        _ZN11AbyssEngine9TransformC1EPS0_(anim, srcAnim);
        F<Transform *>(self, 0x34) = anim;
    }

    _ZN11AbyssEngine6AEMath6VectoraSERKS1_(
        (AEMath::Vector *)((char *)self + 0x50),
        (const AEMath::Vector *)((char *)src + 0x50));

    F<uint8_t>(self, 0x38) = 1;
    F<uint8_t>(self, 0x85) = F<uint8_t>(src, 0x85);
    F<uint8_t>(self, 0x5c) = F<uint8_t>(src, 0x5c);
    F<uint32_t>(self, 0x60) = F<uint32_t>(src, 0x60);
    F<uint32_t>(self, 0x64) = F<uint32_t>(src, 0x64);
    F<uint32_t>(self, 0x68) = F<uint32_t>(src, 0x68);
    F<uint32_t>(self, 0x6c) = F<uint32_t>(src, 0x6c);
    F<uint32_t>(self, 0x78) = F<uint32_t>(src, 0x78);
    F<uint8_t>(self, 0x84) = F<uint8_t>(src, 0x84);
    if (hasNormals != 0) {
        F<uint32_t>(self, 0x70) = F<uint32_t>(src, 0x70);
        F<uint32_t>(self, 0x74) = F<uint32_t>(src, 0x74);
    }
    F<uint32_t>(self, 0x80) = F<uint32_t>(src, 0x80);

    return self;
}

} // namespace AbyssEngine

// Exported under the mangled copy-ctor symbol.
extern "C" void *_ZN11AbyssEngine4MeshC1EPS0_(void *self, void *src)
{
    return AbyssEngine::Mesh_ctor((AbyssEngine::Mesh *)self, (AbyssEngine::Mesh *)src);
}
