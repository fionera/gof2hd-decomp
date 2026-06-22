#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include <type_traits>

class ParticleSettings {
public:
    // ParticleSettings::ParticleSet -- the per-particle preset *index* type. In the original
    // binary this nested type is a 1-byte enum-like selector (Ghidra models it as size 1; the
    // emitter signatures `Array<ParticleSettings::ParticleSet>`,
    // ParticleSystemMesh::finishCurrentTrailParticle(ParticleSettings::ParticleSet, ...),
    // IParticleSystem::setParticleSet(ParticleSettings::ParticleSet) and
    // ParticleSystemManager::addSystem(..., ParticleSettings::ParticleSet, ...) all pass it as a
    // small integer index into the SetDefinition table below). Enumerators are not individually
    // named in the binary (it is used purely as an index), so a placeholder is declared.
    enum ParticleSet {
        ParticleSet_dummy = 0,
        ParticleSet_4 = 4, ParticleSet_7 = 7, ParticleSet_8 = 8, ParticleSet_9 = 9,
        ParticleSet_0xa = 0xa, ParticleSet_0xb = 0xb, ParticleSet_0xc = 0xc, ParticleSet_0xf = 0xf,
        ParticleSet_0x14 = 0x14, ParticleSet_0x15 = 0x15, ParticleSet_0x16 = 0x16,
        ParticleSet_0x17 = 0x17, ParticleSet_0x18 = 0x18, ParticleSet_0x19 = 0x19,
        ParticleSet_0x1a = 0x1a, ParticleSet_0x1b = 0x1b, ParticleSet_0x1c = 0x1c,
        ParticleSet_0x27 = 0x27, ParticleSet_0x2a = 0x2a, ParticleSet_0x2f = 0x2f
    };

    // ParticleSettings::CameraSet -- the camera-set selector passed to the ParticleSystemManager
    // ctor and ParticleSettings::cameraToggle(ParticleSettings::CameraSet). The binary stores it in
    // the manager's `cameraSet` int field and only ever compares it for equality, so it is a small
    // enum; its enumerators are not individually recoverable from the binary (used purely as an
    // opaque selector), so a placeholder is declared.
    enum CameraSet { CameraSet_dummy = 0, CameraSet_1 = 1 };

    // One particle-emitter parameter block (0xa0 bytes). This is the element type of the `sets`
    // table -- NOT the same as the nested `ParticleSet` index enum above. Most slots hold either an
    // int or a float bit pattern depending on the field, so the generic slots stay uint32_t and are
    // reinterpreted where used as floats.
    // Field roles recovered from how IParticleSystem / ParticleSystemSprite / ParticleSystemMesh
    // read this block (they index it by raw `def + 0xNN` pointer arithmetic, so the member NAMES
    // below are private to this TU and safe to rename).
    //
    // This element type is intentionally an *unnamed* (anonymous) struct: in the original binary it
    // has no name for linkage, so its compiler-generated copy-assignment operator is mangled as
    // ParticleSettings::{unnamed type#1}::operator= (_ZN16ParticleSettingsUt_aSERKS0_). That implicit
    // operator= assigns the embedded `name` String then bulk-copies the remaining 0x94 bytes -- which
    // is exactly what the binary does -- so the `sets[d] = sets[s]` copies below emit that symbol.
    struct {
        String name; // +0x00 emitter/texture name
        uint32_t flags; // +0x0c
        int32_t count; // +0x10 particles emitted
        uint32_t lifeBase; // +0x14 base particle lifetime (float bits)
        int32_t lifeRandom; // +0x18 random lifetime added on top
        uint32_t startSize; // +0x1c initial sprite size (float bits)
        uint32_t endSize; // +0x20 final sprite size (float bits)
        uint32_t velocityFromSlot; // +0x24 slot-velocity scale (float bits)
        int32_t lifetime; // +0x28 emitter lifetime / colour-blend duration
        uint32_t flLifetime; // +0x2c emit rate (float bits)
        uint32_t oneShot; // +0x30 ==1 -> emit single burst
        uint32_t color0; // +0x34 start colour (RGBA)
        uint32_t color1; // +0x38 end colour (RGBA)
        int32_t fadeFrames; // +0x3c colour fade frame count
        uint32_t colorFlag; // +0x40 selects per-particle colour mode (float bits)
        int32_t posBase; // +0x44 position spawn base
        int32_t posSpread; // +0x48 position spread
        int32_t ySpread; // +0x4c vertical spread
        int32_t velSpread; // +0x50 velocity spread
        uint32_t field_0x54; // +0x54 unmodelled slot
        uint32_t velBaseX; // +0x58 base velocity X (float bits)
        uint32_t velBaseY; // +0x5c base velocity Y (float bits)
        uint32_t velBaseZ; // +0x60 base velocity Z (float bits)
        uint32_t drag; // +0x64 velocity drag / emitter inheritance (float bits)
        uint32_t velRight; // +0x68 velocity along right axis (float bits)
        uint32_t velUp; // +0x6c velocity along up axis (float bits)
        uint32_t velDir; // +0x70 velocity along facing axis (float bits)
        uint32_t field_0x74; // +0x74 unmodelled slot
        uint32_t posRight; // +0x78 spawn offset along right axis (float bits)
        uint32_t posUp; // +0x7c spawn offset along up axis (float bits)
        uint32_t posDir; // +0x80 spawn offset along facing axis (float bits)
        uint32_t posDirRandom; // +0x84 random spawn offset along facing axis (float bits)
        uint32_t uvU0; // +0x88 texture-rect u0 (float bits)
        uint32_t uvV0; // +0x8c texture-rect v0 (float bits)
        uint32_t uvU1; // +0x90 texture-rect u1 (float bits)
        uint32_t uvV1; // +0x94 texture-rect v1 (float bits)
        int32_t speedThreshold; // +0x98 minimum emitter speed^2 to emit
        int32_t frames; // +0x9c animation frame count
    } sets[48];

    // Spelling for the anonymous element type above, so this TU can name `SetDefinition *` locals
    // without giving the type a linkage name (which would change the operator= mangling).
    using SetDefinition = std::remove_reference<decltype(sets[0])>::type;

    ParticleSettings();

    ~ParticleSettings();

    // Populate the 48 SetDefinition entries with the built-in emitter presets.
    int init();

    // Scale lifetimes and colour components of every SetDefinition by `scale` (or its
    // reciprocal, depending on per-set flag bits).
    void multiplyAll(float scale);

    // out = b*t + (1-t)*a, field by field, over the lifetime/colour/scale slots.
    void Interpolate(ParticleSet a, ParticleSet b, float t, ParticleSet out);
};
#endif
