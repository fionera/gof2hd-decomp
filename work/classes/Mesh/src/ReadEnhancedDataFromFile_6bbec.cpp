#include "class.h"

// AbyssEngine::Mesh::ReadEnhancedDataFromFile(unsigned int file, unsigned int flags)
// Reads the "enhanced" mesh record: bounding sphere, then a sequence of animation tracks
// (position, rotation, scale, pivot, and -- when flags request -- extra material/uv tracks)
// into a freshly allocated Transform. Each track is either one vector channel (type 1) or
// three scalar channels (type 0); each sample updates a global "max value" bound and is fed
// to Transform::InsertKeyFrame under a per-track channel mask. On any short read the partial
// Transform is destroyed and the routine returns -1; on success it is attached at +0x34 and
// its animation time range is configured.

namespace AbyssEngine {

struct Transform;

extern "C" int   _ZN11AbyssEngine6AEFile4ReadEjPvj(int size, void *dst, unsigned int file);
extern "C" void *_ZN11AbyssEngine9TransformC1Ev(Transform *self);
extern "C" void *_ZN11AbyssEngine9TransformD1Ev(Transform *self);
extern "C" void  _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(Transform *self, unsigned int ch, int frame);
extern "C" void  _ZN11AbyssEngine9Transform23SetAnimationRangeInTimeEx(Transform *self, long long range);

// Global "max value seen" accumulators (one per track family). The original tracks the
// running maximum of each channel into these PC-relative float slots.
__attribute__((visibility("hidden"))) extern float *g_maxA;  // position
__attribute__((visibility("hidden"))) extern float *g_maxB;
__attribute__((visibility("hidden"))) extern float *g_maxC;  // rotation
__attribute__((visibility("hidden"))) extern float *g_maxD;
__attribute__((visibility("hidden"))) extern float *g_maxE;  // scale
__attribute__((visibility("hidden"))) extern float *g_maxF;
__attribute__((visibility("hidden"))) extern float *g_maxG;  // pivot
__attribute__((visibility("hidden"))) extern float *g_maxH;  // material/uv
__attribute__((visibility("hidden"))) extern float *g_animRate;
__attribute__((visibility("hidden"))) extern float  g_uvDiv, g_uvMulA, g_uvDivB;

static inline void bumpMax(float *slot, float v) {
    if (v > 0.0f && *slot < v)
        *slot = v;
}

// Reads one "type 1" vector track: count u16, then for each entry a leading float key and
// a 12-byte vector payload, inserting a keyframe under `channel`.
static bool readVectorTrack(int file, Transform *anim, float *maxSlot, unsigned int channel) {
    union { unsigned short u16; float f; unsigned char b[12]; } key;
    unsigned char vec[12];
    unsigned short count;
    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &count, file) == 0)
        return false;
    for (unsigned int i = 0; i < count; ++i) {
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &key.f, file) == 0)
            return false;
        bumpMax(maxSlot, key.f);
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(0xc, vec, file) == 0)
            return false;
        _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, channel, (int)key.f);
    }
    return true;
}

// Reads one "type 0" scalar-triplet track: three axes, each a u16 count then per-sample a
// value float and a key float. `chX/chY/chZ` are the channel masks for axes 0/1/2; when
// `negateY` is set the Y axis value is negated before insert (matching the original).
static bool readScalarTrack(int file, Transform *anim, float *maxSlot,
                            unsigned int ch0, unsigned int ch1, unsigned int ch2,
                            bool negateY) {
    for (unsigned int axis = 0; axis < 3; ++axis) {
        unsigned short count;
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &count, file) == 0)
            return false;
        for (unsigned int j = 0; j < count; ++j) {
            float value;
            if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &value, file) == 0)
                return false;
            bumpMax(maxSlot, value);
            float key;
            if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &key, file) == 0)
                return false;
            if (axis == 2) {
                _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, ch2, (int)value);
            } else if (axis == 1) {
                if (negateY) key = -key;
                _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, ch1, (int)value);
            } else {
                _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, ch0, (int)value);
            }
        }
    }
    return true;
}

int Mesh_ReadEnhancedDataFromFile(void *self, unsigned int file, unsigned int flags)
{
    Transform *anim = (Transform *)operator new(0x180);
    _ZN11AbyssEngine9TransformC1Ev(anim);

    // Bounding sphere: cx, cy, cz, radius. The cy/cz pair is swapped and cz negated.
    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, (char *)self + 0x3c, file) == 0) goto fail;
    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, (char *)self + 0x40, file) == 0) goto fail;
    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, (char *)self + 0x44, file) == 0) goto fail;
    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, (char *)self + 0x48, file) == 0) goto fail;
    {
        float t = F<float>(self, 0x40);
        F<float>(self, 0x40) = F<float>(self, 0x44);
        F<float>(self, 0x44) = -t;
    }

    // Track family header is a u16 "type" (0=scalar triplet, 1=vector).
    {
        unsigned short type;

        // --- Position track ---
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxB, 7)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxA, 1, 4, 2, true)) goto fail;
        }

        // --- Rotation track ---
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxD, 0x1c0)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxC, 0x40, 0x80, 0x100, false)) goto fail;
        }

        // --- Scale track ---
        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &type, file) == 0) goto fail;
        if (type == 1) {
            if (!readVectorTrack(file, anim, g_maxF, 0x38)) goto fail;
        } else if (type == 0) {
            if (!readScalarTrack(file, anim, g_maxE, 8, 0x10, 0x20, false)) goto fail;
        }

        // --- Optional pivot track (flags & 0x18) ---
        if ((flags & 0x18) != 0) {
            if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &type, file) == 0) goto fail;
            if (type == 2) {
                unsigned short count;
                if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &count, file) == 0) goto fail;
                for (unsigned int i = 0; i < count; ++i) {
                    float value;
                    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &value, file) == 0) goto fail;
                    bumpMax(g_maxG, value);
                    float key;
                    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &key, file) == 0) goto fail;
                    _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, 0x200, (int)value);
                }
            }
        }

        // --- Optional material / uv track (flags & 0x10): 7 channels ---
        if ((flags & 0x10) != 0) {
            unsigned short outer;
            if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &outer, file) == 0) goto fail;
            if (outer != 0) {
                static const unsigned int chans[7] = {
                    0x400, 0x800, 0x2000, 0x4000, 0, 0, 0x40000
                };
                for (unsigned int c = 0; c < 7; ++c) {
                    unsigned short count;
                    if (_ZN11AbyssEngine6AEFile4ReadEjPvj(2, &count, file) == 0) goto fail;
                    for (int j = 0; j < (short)count; ++j) {
                        float value;
                        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &value, file) == 0) goto fail;
                        bumpMax(g_maxH, value);
                        float key;
                        if (_ZN11AbyssEngine6AEFile4ReadEjPvj(4, &key, file) == 0) goto fail;
                        key = key / g_uvDiv;
                        if (c == 6)
                            key = (key * g_uvMulA) / g_uvDivB;
                        _ZN11AbyssEngine9Transform14InsertKeyFrameEjf(anim, chans[c], (int)value);
                        F<uint8_t>(self, 0x85) = 1;
                    }
                }
            }
        }
    }

    // Finalize: attach the track if it has frames, else discard it.
    if (F<int>(anim, 0x11c) < 1) {
        operator delete(_ZN11AbyssEngine9TransformD1Ev(anim));
    } else {
        F<Transform *>(self, 0x34) = anim;
        float rate = *g_animRate;
        F<int>(anim, 0xe8) = (int)rate;
        _ZN11AbyssEngine9Transform23SetAnimationRangeInTimeEx(anim, (long long)rate);
    }
    return 1;

fail:
    operator delete(_ZN11AbyssEngine9TransformD1Ev(anim));
    return -1;
}

} // namespace AbyssEngine

extern "C" void _ZN11AbyssEngine4Mesh24ReadEnhancedDataFromFileEjj(
    void *self, unsigned int file, unsigned int flags)
{
    AbyssEngine::Mesh_ReadEnhancedDataFromFile(self, file, flags);
}
