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
struct Player;
struct Transform;

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr) __attribute__((nothrow));

// PlayerStation derives from PlayerStaticFar (a stationary KIPlayer subclass). The
// inherited base subobject occupies +0x00..+0x140; only the fields the recovered
// methods touch are named, the rest of the base layout is preserved as padding so
// the byte offsets stay correct. Trailing comments are the original field offsets.
class PlayerStation {
public:
    void *vtable;                        // +0x00  vtable pointer (base)
    Player *player;                      // +0x04  owned Player base subobject
    byte  _pad_0x08[0x25 - 0x08];        // +0x08
    uint8_t field_0x25;                  // +0x25
    byte  _pad_0x26[0x58 - 0x26];        // +0x26
    float posX;                          // +0x58  init position x
    float posY;                          // +0x5c  init position y
    float posZ;                          // +0x60  init position z
    byte  _pad_0x64[0x71 - 0x64];        // +0x64
    uint8_t field_0x71;                  // +0x71
    byte  _pad_0x72[0xf5 - 0x72];        // +0x72
    uint8_t visible;                     // +0xf5  render-enabled flag
    byte  _pad_0xf6[0x130 - 0xf6];       // +0xf6
    Array<BoundingVolume *> *boundingVolumes; // +0x130 collision volumes (inherited base member)
    byte  _pad_0x134[0x140 - 0x134];     // +0x134
    // ---- PlayerStation-specific fields ----
    AEGeometry *rootGeometry;            // +0x140  owned root geometry (mesh tree)
    uint32_t field_0x144;                // +0x144  transform handle (stationIndex 100 anim)
    int32_t stationIndex;                // +0x148  station model index
    AEGeometry *secondGeometry;          // +0x14c  secondary owned geometry
    uint32_t collisionIndex;             // +0x150  last colliding volume index
    int32_t collisionRadius;             // +0x154  outer collision radius
    uint32_t field_0x158;                // +0x158
    uint32_t field_0x15c;                // +0x15c
    uint32_t field_0x160;                // +0x160
    uint32_t field_0x164;                // +0x164  sub-mesh transform handle (stationIndex 0x6c)
    uint32_t field_0x168;                // +0x168  sub-mesh transform handle
    uint32_t field_0x16c;                // +0x16c  sub-mesh transform handle
    uint32_t field_0x170;                // +0x170  sub-mesh transform handle

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

    // Complete-object destructor body (D2): runs the teardown and yields `this`
    // so the deleting destructor can chain operator delete onto it.
    void *destructor_body();

    // Animation-transform plumbing used by update() for multi-mesh stations
    // (stationIndex 0x6c). transformGet fetches a packed Transform handle for a
    // sub-mesh from the canvas; transformUpdate advances it by `delta`.
    long long transformGet(void *canvas, int meshId);
    void transformUpdate(uint32_t lo, uint32_t hi, int delta, int deltaHigh, int flag);
};
#endif
