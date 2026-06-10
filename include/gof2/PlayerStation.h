#ifndef GOF2_PLAYERSTATION_H
#define GOF2_PLAYERSTATION_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct Station;
struct AEGeometry;
struct BoundingVolume;
struct FileRead;
struct PaintCanvas;
struct Transform;

struct PlayerStaticFar {
    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStaticFar() noexcept(false);
};







static inline void *&P(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr) __attribute__((nothrow));

struct PlayerStation {
    uint8_t field_0x25;                 // +0x25
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
    float field_0x60;                   // +0x60
    uint8_t field_0x71;                 // +0x71
    uint8_t field_0xf5;                 // +0xf5
    uint32_t field_0x130;               // +0x130
    uint32_t field_0x140;               // +0x140
    int32_t field_0x144;                // +0x144
    int32_t field_0x148;                // +0x148
    uint32_t field_0x14c;               // +0x14c
    uint32_t field_0x150;               // +0x150
    int32_t field_0x154;                // +0x154
    uint32_t field_0x158;               // +0x158
    uint32_t field_0x15c;               // +0x15c
    uint32_t field_0x160;               // +0x160
    int32_t field_0x164;                // +0x164
    int32_t field_0x168;                // +0x168
    int32_t field_0x16c;                // +0x16c
    int32_t field_0x170;                // +0x170

    PlayerStation(Station *station);
    ~PlayerStation() noexcept(false);
    void setVisible(bool visible);
    void setPosition(const Vector &position);
    Vector projectCollisionOnSurface(const Vector &position);
    void *getRoot();
    Vector getProjectionVector(const Vector &position);
    void render();
    void outerCollide(const Vector &position);
    bool outerCollide(float x, float y, float z);
    Vector getPosition();
    bool collide(float x, float y, float z);
    void update(int delta);
    void translate(float x, float y, float z);

    // ---- methods (converted from free functions) ----
    void deleting_dtor();
};
#endif
