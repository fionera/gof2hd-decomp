#include "gof2/Mesh.h"
#include "gof2/AEMath.h"
#include "gof2/AEFile.h"
#include <new>

// gof2/Transform.h is intentionally NOT included here: it carries its own minimal
// `AbyssEngine::Mesh` view (for Transform's own needs) that would clash with the full
// Mesh definition in gof2/Mesh.h. Mesh.cpp only touches a handful of Transform members,
// which the layout-only AbyssEngine::Transform view in gof2/AEMath.h already exposes, so
// every `anim->...` below is a genuine call into the engine's real Transform.
namespace AbyssEngine {

// AbyssEngine::MeshConvertToVBO(Mesh*) — defined in AbyssEngine.cpp; flushes a mesh's
// CPU arrays into GL buffer objects.
int MeshConvertToVBO(Mesh *mesh);

// Module-level flag: when set, meshes carry the extra per-vertex tangent/binormal data,
// so the copy constructor must propagate those arrays as well.
__attribute__((visibility("hidden"))) extern unsigned char *g_hasNormalsFlag;

// AbyssEngine::Mesh::Mesh(Mesh*) — copy constructor.
// Resets the embedded bounding sphere / Vector to their defaults, then (when given a
// source) flushes the source's pending VBO upload, copies the bounding sphere and the
// vertex-format header, shares the CPU array pointers (the tangent/binormal pair only
// when the global tangent flag is set), deep-copies the animation Transform, copies the
// embedded Vector and the remaining scalar/GL-handle fields, and marks the new mesh as a
// shared alias so it will not free those arrays.
Mesh::Mesh(Mesh *src) {
    Mesh *self = this;

    // Default-initialize the embedded bounding sphere (+0x3c) and Vector (+0x50).
    self->boundsCenterX = 0.0f;
    self->boundsCenterY = 0.0f;
    self->boundsCenterZ = 1.0f;
    self->boundsRadius = 0.0f;
    self->boundsRadiusSq = 0.0f;
    self->field_0x50 = 0.0f;
    self->field_0x54 = 1.0f;

    if (src == 0)
        return;

    // Make sure the source's arrays are resident in GL buffers before sharing them.
    if (src->vboEligible != 0)
        MeshConvertToVBO(src);

    AEMath::BSphere &dstSphere = *reinterpret_cast<AEMath::BSphere *>(&self->boundsCenterX);
    const AEMath::BSphere &srcSphere = *reinterpret_cast<const AEMath::BSphere *>(&src->boundsCenterX);
    dstSphere = srcSphere;

    const bool hasTangents = (*g_hasNormalsFlag != 0);

    self->vboByteSize = 0;
    self->vertexFormat = src->vertexFormat;
    self->materialId = 0;
    self->field_0x20 = 0;
    self->vertexCount = src->vertexCount;
    self->positions = src->positions;
    self->texCoords = src->texCoords;
    self->colors = src->colors;
    self->normals = src->normals;
    if (hasTangents) {
        self->tangents = src->tangents;
        self->binormals = src->binormals;
    }
    self->indexCount = src->indexCount;
    self->field_0x2a = src->field_0x2a;
    self->indices = src->indices;
    self->field_0x30 = src->field_0x30;

    Transform *srcAnim = src->animation;
    if (srcAnim == 0) {
        self->animation = 0;
    } else {
        // Deep-copy the animation track via the Transform copy constructor.
        Transform *copy = static_cast<Transform *>(operator new(0x180));
        new (static_cast<void *>(copy)) Transform(srcAnim);
        self->animation = copy;
    }

    AEMath::Vector &dstVec = *reinterpret_cast<AEMath::Vector *>(&self->field_0x50);
    const AEMath::Vector &srcVec = *reinterpret_cast<const AEMath::Vector *>(&src->field_0x50);
    dstVec = srcVec;

    self->shared = 1;
    self->hasAnimation = src->hasAnimation;
    self->uploaded = src->uploaded;
    self->positionVBO = src->positionVBO;
    self->indexVBO = src->indexVBO;
    self->texCoordVBO = src->texCoordVBO;
    self->normalVBO = src->normalVBO;
    self->colorVBO = src->colorVBO;
    self->vboEligible = src->vboEligible;
    if (hasTangents) {
        self->tangentVBO = src->tangentVBO;
        self->binormalVBO = src->binormalVBO;
    }
    self->enhancedData = src->enhancedData;
}

// AbyssEngine::Mesh::ReadEnhancedDataFromFile(unsigned int file, unsigned int flags)
// Reads the "enhanced" mesh record from an open AEFile handle: the bounding sphere first,
// then a sequence of animation tracks (position, rotation, scale, then -- when the flags
// request them -- a pivot track and a 7-channel material/uv track) into a freshly
// allocated animation Transform. Each track is either one vector channel (header type 1)
// or three scalar channels (header type 0); every sample feeds Transform::InsertKeyFrame
// under a per-track channel mask and updates a module-level running maximum. On any short
// read the partial Transform is destroyed and -1 is returned; on success the Transform is
// attached at +0x34 (when it gathered any frames) and its animation time range is set.
// Module-level running maxima (one slot per track family) kept by the engine; the loader
// raises each toward the largest sample it sees. They are genuinely external engine state
// (PC-relative data slots in the original), so they remain module-level globals here.
__attribute__((visibility("hidden"))) extern float *g_maxA;  // position scalar
__attribute__((visibility("hidden"))) extern float *g_maxB;  // position vector
__attribute__((visibility("hidden"))) extern float *g_maxC;  // rotation scalar
__attribute__((visibility("hidden"))) extern float *g_maxD;  // rotation vector
__attribute__((visibility("hidden"))) extern float *g_maxE;  // scale scalar
__attribute__((visibility("hidden"))) extern float *g_maxF;  // scale vector
__attribute__((visibility("hidden"))) extern float *g_maxG;  // pivot
__attribute__((visibility("hidden"))) extern float *g_maxH;  // material/uv
__attribute__((visibility("hidden"))) extern float *g_animRate;
__attribute__((visibility("hidden"))) extern float  g_uvDiv, g_uvMulA, g_uvDivB;

namespace {

inline void bumpMax(float *slot, float v) {
    if (v > 0.0f && *slot < v)
        *slot = v;
}

// Reads one "type 1" vector track: u16 count, then per entry a float key followed by a
// 12-byte vector payload, inserting a keyframe under `channel`.
bool readVectorTrack(unsigned int file, Transform *anim, float *maxSlot, unsigned int channel) {
    unsigned short count;
    if (AEFile::Read(2, &count, file) == 0)
        return false;
    for (unsigned int i = 0; i < count; ++i) {
        float key;
        if (AEFile::Read(4, &key, file) == 0)
            return false;
        bumpMax(maxSlot, key);
        unsigned char vec[12];
        if (AEFile::Read(0xc, vec, file) == 0)
            return false;
        anim->InsertKeyFrame(channel, key);
    }
    return true;
}

// Reads one "type 0" scalar-triplet track: three axes, each a u16 count then per sample a
// value float and a key float. `ch0/ch1/ch2` are the per-axis channel masks; when
// `negateMiddle` is set the middle axis' value is negated before insert.
bool readScalarTrack(unsigned int file, Transform *anim, float *maxSlot,
                     unsigned int ch0, unsigned int ch1, unsigned int ch2,
                     bool negateMiddle) {
    for (unsigned int axis = 0; axis < 3; ++axis) {
        unsigned short count;
        if (AEFile::Read(2, &count, file) == 0)
            return false;
        for (unsigned int j = 0; j < count; ++j) {
            float value;
            if (AEFile::Read(4, &value, file) == 0)
                return false;
            bumpMax(maxSlot, value);
            float key;
            if (AEFile::Read(4, &key, file) == 0)
                return false;
            if (axis == 2) {
                anim->InsertKeyFrame(ch2, value);
            } else if (axis == 1) {
                if (negateMiddle) value = -value;
                anim->InsertKeyFrame(ch1, value);
            } else {
                anim->InsertKeyFrame(ch0, value);
            }
        }
    }
    return true;
}

} // namespace

int Mesh::ReadEnhancedDataFromFile(unsigned int file, unsigned int flags) {
    Mesh *self = this;
    Transform *anim = static_cast<Transform *>(operator new(0x180));
    new (static_cast<void *>(anim)) Transform();

    // Bounding sphere: center x/y/z and radius. The y/z pair is swapped and z negated to
    // convert from the file's coordinate convention to the engine's.
    if (AEFile::Read(4, &self->boundsCenterX, file) == 0) goto fail;
    if (AEFile::Read(4, &self->boundsCenterY, file) == 0) goto fail;
    if (AEFile::Read(4, &self->boundsCenterZ, file) == 0) goto fail;
    if (AEFile::Read(4, &self->boundsRadius, file) == 0) goto fail;
    {
        float y = self->boundsCenterY;
        self->boundsCenterY = self->boundsCenterZ;
        self->boundsCenterZ = -y;
    }

    {
        unsigned short type;

        // --- Position track ---
        if (AEFile::Read(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxB, 7)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxA, 1, 4, 2, true)) goto fail;
        }

        // --- Rotation track ---
        if (AEFile::Read(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxD, 0x1c0)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxC, 0x40, 0x80, 0x100, false)) goto fail;
        }

        // --- Scale track ---
        if (AEFile::Read(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxF, 0x38)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxE, 8, 0x10, 0x20, false)) goto fail;
        }

        // --- Optional pivot track (flags & 0x18, header type 2) ---
        if ((flags & 0x18) != 0) {
            if (AEFile::Read(2, &type, file) == 0) goto fail;
            if (type == 2) {
                unsigned short count;
                if (AEFile::Read(2, &count, file) == 0) goto fail;
                for (unsigned int i = 0; i < count; ++i) {
                    float value;
                    if (AEFile::Read(4, &value, file) == 0) goto fail;
                    bumpMax(g_maxG, value);
                    float key;
                    if (AEFile::Read(4, &key, file) == 0) goto fail;
                    anim->InsertKeyFrame(0x200, value);
                }
            }
        }

        // --- Optional material / uv track (flags & 0x10): up to 7 channels ---
        if ((flags & 0x10) != 0) {
            unsigned short present;
            if (AEFile::Read(2, &present, file) == 0) goto fail;
            if (present != 0) {
                static const unsigned int kChannels[7] = {
                    0x400, 0x800, 0x2000, 0x4000, 0, 0, 0x40000
                };
                for (unsigned int c = 0; c < 7; ++c) {
                    unsigned short count;
                    if (AEFile::Read(2, &count, file) == 0) goto fail;
                    for (int j = 0; j < (short)count; ++j) {
                        float value;
                        if (AEFile::Read(4, &value, file) == 0) goto fail;
                        bumpMax(g_maxH, value);
                        float key;
                        if (AEFile::Read(4, &key, file) == 0) goto fail;
                        key = key / g_uvDiv;
                        if (c == 6)
                            key = (key * g_uvMulA) / g_uvDivB;
                        anim->InsertKeyFrame(kChannels[c], value);
                        self->hasAnimation = 1;
                    }
                }
            }
        }
    }

    // Attach the track if it gathered any frames, otherwise discard it.
    if (anim->keyFrameCount < 1) {
        anim->~Transform();
        operator delete(anim);
    } else {
        self->animation = anim;
        float rate = *g_animRate;
        anim->animationStart = (int)rate;
        anim->SetAnimationRangeInTime((long long)rate, (long long)rate);
    }
    return 1;

fail:
    anim->~Transform();
    operator delete(anim);
    return -1;
}

} // namespace AbyssEngine
