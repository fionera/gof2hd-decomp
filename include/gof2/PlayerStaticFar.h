#ifndef GOF2_PLAYERSTATICFAR_H
#define GOF2_PLAYERSTATICFAR_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PlayerStaticFar class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class itself) but its API uses
// AbyssEngine::AEMath::Vector. Field offsets recovered per-method from the
// target disassembly; accessed via byte-offset casts from `this`.


namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct AEGeometry;

// PlayerStaticFar derives from PlayerStatic; only the fields touched by the
// recovered methods are named, the rest of the base layout is preserved as
// padding so the byte offsets stay correct (0x140 bytes total).
struct PlayerStaticFar {
    void *field_0x00;            // +0x00  vtable pointer
    void *field_0x04;            // +0x04  Player*
    void *field_0x08;            // +0x08  AEGeometry*
    byte  _pad_0x0c[0x58-0x0c];  // +0x0c
    float field_0x58;            // +0x58  init position x
    float field_0x5c;            // +0x5c  init position y
    float field_0x60;            // +0x60  init position z
    byte  _pad_0x64[0x90-0x64];  // +0x64
    float field_0x90;            // +0x90  camera position (Vec3.x)
    float field_0x94;            // +0x94  camera position (Vec3.y)
    float field_0x98;            // +0x98  camera position (Vec3.z)
    float field_0x9c;            // +0x9c  object float position (Vec3.x)
    float field_0xa0;            // +0xa0  object float position (Vec3.y)
    float field_0xa4;            // +0xa4  object float position (Vec3.z)
    byte  _pad_0xa8[0x124-0xa8]; // +0xa8
    int   field_0x124;           // +0x124 integer position x
    int   field_0x128;           // +0x128 integer position y
    int   field_0x12c;           // +0x12c integer position z
    void *field_0x130;           // +0x130 bounding-volume container
    int   field_0x134;           // +0x134 view direction (Vec3.x)
    int   field_0x138;           // +0x138 view direction (Vec3.y)
    int   field_0x13c;           // +0x13c view direction (Vec3.z)

    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);
    Vector getProjectionVector(const Vector &value);
    void render();
    Vector projectCollisionOnSurface(const Vector &value);
    bool outerCollide(float x, float y, float z);
    void outerCollide(Vector value);
    Vector getInitPosition(Vector);
    void update(int delta);
    bool collide(float x, float y, float z);
};
#endif
